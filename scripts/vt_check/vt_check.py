############################################################
# Run: python vt_check
# Dependencies: sha256sum in Linux
# Description: Batch Check in VirusTotal
# Author: Ky9oss
############################################################

import requests
import os
from cryptography.hazmat.primitives import hashes
from pathlib import Path
import subprocess

# def sha256_hash(data: bytes) -> bytes:
#     digest = hashes.Hash(hashes.SHA256())
#     digest.update(data)
#     return digest.finalize()

# def file_hash():
# digest = hashes.Hash(algorithm)
# chunk_size = 8192 * 8  # 64KB
#
# with open(filepath, "rb") as f:
#     while chunk := f.read(chunk_size):
#         digest.update(chunk)
#
# return digest.finalize().hex()

report = "../../temp/result.txt"


def file_hash(filepath: str) -> str:
    result = subprocess.run(["sha256sum", filepath], capture_output=True, text=True)
    return result.stdout.split()[0]


def get_apikey() -> str:
    with open("apikey.txt", "r") as file:
        return file.readline().strip()


def hash2report(hash: str, apikey: str, filename: str) -> str:
    url = "https://www.virustotal.com/api/v3/files/" + hash
    headers = {"accept": "application/json", "x-apikey": apikey}
    response = requests.get(url, headers=headers)
    response = response.json()
    if "error" in response:
        with open(report, "a") as file:
            file.write(
                "[FILE-NOT-FOUND]\t"
                + filename
                + "\t"
                + hash
                + "(Hash)\t"
                + str(response["error"]["code"])
                + "\n"
            )
    elif "data" in response:
        last_analysis_stats = response["data"]["attributes"]["last_analysis_stats"]
        total_votes = response["data"]["attributes"]["total_votes"]
        if (
            last_analysis_stats["malicious"] > 0
            or last_analysis_stats["suspicious"] > 0
            or total_votes["harmless"] > 0
            or total_votes["malicious"] > 0
        ):
            with open(report, "a") as file:
                file.write(
                    "[MIGHT-BE-MALWARE]\t"
                    + filename
                    + "\t"
                    + hash
                    + "(Hash)\t"
                    + str(last_analysis_stats)
                    + "\t"
                    + str(total_votes)
                    + "\n"
                )
    else:
        with open(report, "a") as file:
            file.write("[UNKNOWN-ERRRO]\t" + str(response) + "\n")


files = []


def get_all_files(directory: str) -> list:

    global files

    entries = Path(directory).iterdir()
    for entry in entries:
        if entry.is_dir() == True:
            get_all_files(directory + "/" + entry.name)
        elif entry.is_file() == True:
            files.append(directory + "/" + entry.name)
        else:
            print("error")


# def file_filter():


def find_key(obj, target_key, path=""):
    results = []

    if isinstance(obj, dict):
        for k, v in obj.items():
            new_path = f"{path}.{k}" if path else k

            if k == target_key:
                results.append(new_path)

            results.extend(find_key(v, target_key, new_path))

    elif isinstance(obj, list):
        for i, item in enumerate(obj):
            new_path = f"{path}[{i}]"
            results.extend(find_key(item, target_key, new_path))

    return results


def main():
    global files

    apikey = get_apikey()

    get_all_files("../../temp/AS2753")

    hash_list = []

    for file in files:
        if file.endswith(
            (".txt", ".png", ".jpg", ".jpeg", ".pak", ".md", ".tjs", ".sig")
        ):
            continue
        hash = file_hash(file)
        if hash not in hash_list:
            hash_list.append(hash)
            print(file)
            hash2report(hash, apikey, file)

    print("Get " + str(len(hash_list)) + " unique files.")


main()


# 'last_analysis_stats': {'malicious': 0, 'suspicious': 0, 'undetected': 71, 'harmless': 0, 'timeout': 0, 'confirmed-timeout': 0, 'failure': 0, 'type-unsupported': 4}, 'total_votes': {'harmless': 0, 'malicious': 0},

# print(find_key(obj, 'last_analysis_stats'))
# print(find_key(obj, 'total_votes'))
# ['data.attributes.last_analysis_stats']
# ['data.attributes.total_votes']
