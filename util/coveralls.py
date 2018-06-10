#!/usr/bin/env python3

import re
import os.path
import hashlib
import os
import argparse
import subprocess
import json
import sys
import requests


def executeGcov(gcov, source_root, dirname, basename):
    result = subprocess.run([gcov, '-i', '-b', '-s', source_root, '-r', basename], cwd=dirname)
    if result.stdout is not None:
        print(result.stdout)
    if result.stderr is not None:
        print(result.stderr, file=sys.stderr)
    if result.returncode != 0:
        print('error: An error occurred while executing the following command `'
              + ' '.join(result.args) + '\'.',
              file=sys.stderr)
        sys.exit(1)
    gcov_file = os.path.join(dirname, basename) + '.gcov'
    if not os.path.isfile(gcov_file):
        print('error: Could not find the resulting .gcov file.', file=sys.stderr)
        sys.exit(1)
    return gcov_file

def parseGcovIntermediateFormat(source_root, gcov_file, source_path_to_coverage):
    coverage = None
    with open(gcov_file) as f:
        gcov_line_number = 0
        branch_line = None
        branch_number = None
        for line in f:
            gcov_line_number += 1
            line = line.rstrip('\n')
            if re.match('version:', line):
                continue
            m = re.match('file:(.*)$', line)
            if m:
                source_path = m.group(1)
                source_path = os.path.relpath(source_path, source_root)
                if source_path not in source_path_to_coverage:
                    source_path_to_coverage[source_path] = {
                        'coverage': [],
                        'branches': {},
                    }
                coverage = source_path_to_coverage[source_path]
                continue
            if re.match('function:', line):
                continue
            m = re.match('lcount:(\\d+),(\\d+),(\\d+)$', line)
            if m:
                line_number = int(m.group(1))
                execution_count = int(m.group(2))
                has_unexecuted_block = int(m.group(3))
                if has_unexecuted_block == 1:
                    pass
                    #execution_count = 0
                if line_number > len(coverage['coverage']):
                    old_length = len(coverage['coverage'])
                    coverage['coverage'].extend([None] * (line_number - old_length))
                    assert(len(coverage['coverage']) == line_number)
                    coverage['coverage'][line_number - 1] = execution_count
                else:
                    if coverage['coverage'][line_number - 1] is None:
                        coverage['coverage'][line_number - 1] = execution_count
                    else:
                        coverage['coverage'][line_number - 1] += execution_count
                continue
            m = re.match('branch:(\\d+),(notexec|taken|nottaken)$', line)
            if m:
                line_number = int(m.group(1))
                branch_coverage_type = m.group(2)
                if line_number != branch_line:
                    branch_line = line_number
                    branch_number = 0
                else:
                    branch_number += 1
                if (branch_line, branch_number) not in coverage['branches']:
                    coverage['branches'][(branch_line, branch_number)] = 0
                if branch_coverage_type == 'taken':
                    coverage['branches'][(branch_line, branch_number)] += 1
                continue
            print('{0}:{1}: {2}: An unknown error occurred.'.format(gcov_file, gcov_line_number, line))
            sys.exit(1)

def createSourceDigest(source_path):
    hash_md5 = hashlib.md5()
    with open(source_path, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

def report(job):
    url = 'https://coveralls.io/api/v1/jobs'
    response = requests.post(url, files={'json_file': json.dumps(job)})
    try:
        result = response.json()
    except ValueError:
        result = {
            'error': 'Failure to submit data. '
            'Response [%(status)s]: %(text)s' % {
                'status': response.status_code,
                'text': response.text
            }
        }
        print(result, file=sys.stderr)
        sys.exit(1)
    if 'error' in result:
        print('error: An error occurred while posting the coverage data.',
              file=sys.stderr)
        print(json.dumps(result), file=sys.stderr)
        sys.exit(1)

def parseArgs(argv):
    parser = argparse.ArgumentParser('coveralls.py')
    parser.add_argument('--source-root', metavar='DIR', required=True,
                        help='the root directory of the source tree')
    parser.add_argument('--build-root', metavar='DIR', required=True,
                        help='the root directory of the build tree')
    parser.add_argument('--with-gcov', metavar='FILE', default='gcov',
                        help='the command or path to gcov executable')
    parser.add_argument('--clear', action='store_true',
                        help='clear all .gcov files in the build tree before execution')
    parser.add_argument('--repo-token', metavar='TOKEN',
                        help='repo_token for the repository')
    parser.add_argument('--service-name', metavar='STRING',
                        help='the CI service or other environment in which the test suite was run')
    parser.add_argument('--service-number', metavar='STRING',
                        help='the build number')
    parser.add_argument('--service-job-id', metavar='STRING',
                        help='a unique identifier of the job on the service specified by `--service-name\'')
    parser.add_argument('--service-pull-request', metavar='STRING',
                        help='the associated pull request ID of the build')
    param = parser.parse_args(argv[1:])

    if not os.path.isdir(param.source_root):
        print('error: `{0}\' specified by `--source-root\' is not a directory.',
              file=sys.stderr)
        sys.exit(1)
    param.source_root = os.path.realpath(param.source_root)

    if not os.path.isdir(param.build_root):
        print('error: `{0}\' specified by `--build-root\' is not a directory.',
              file=sys.stderr)
        sys.exit(1)
    param.build_root = os.path.realpath(param.build_root)

    return param

def main(argv):
    param = parseArgs(argv)

    if param.clear:
        for dirpath, dirnames, filenames in os.walk(param.build_root):
            for filename in filenames:
                if (re.search('\\.gcov$', filename)):
                    path = os.path.join(dirpath, filename)
                    os.remove(path)
                    print('removed {0}'.format(path))

    gcov_files = []
    for dirpath, dirnames, filenames in os.walk(param.build_root):
        for filename in filenames:
            if (re.search('\\.o$', filename)):
                gcov_file = executeGcov(param.with_gcov, param.source_root, dirpath, filename)
                gcov_files.append(gcov_file)

    source_path_to_coverage = {}
    for gcov_file in gcov_files:
        parseGcovIntermediateFormat(param.source_root, gcov_file, source_path_to_coverage)

    source_files = []
    for source_path, coverage in source_path_to_coverage.items():
        source_abs_path = os.path.join(param.source_root, source_path)
        branches = []
        for key, hits in coverage['branches'].items():
            line_number, branch_number = key
            branches.append([line_number, 0, branch_number, hits])
        branches.sort(key=lambda branch: (branch[0], branch[2]))
        sorted_branches = []
        for branch in branches:
            sorted_branches.extend(branch)
        source_files.append({
            'name': source_path,
            'source_digest': createSourceDigest(source_abs_path),
            'coverage': coverage['coverage'],
            #'branches': sorted_branches,
        })

    job = {}
    if param.repo_token is not None:
        job['repo_token'] = param.repo_token
    if param.service_name is not None:
        job['service_name'] = param.service_name
    if param.service_number is not None:
        job['service_number'] = param.service_number
    if param.service_job_id is not None:
        job['service_job_id'] = param.service_job_id
    if param.service_pull_request is not None:
        job['service_pull_request'] = param.service_pull_request
    job['source_files'] = source_files
    report(job)
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
