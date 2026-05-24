import requests
import re
import json
import os
import json
import itertools
from collections import defaultdict
from collections import OrderedDict
from datetime import datetime
from urllib.parse import urlencode
from requests.adapters import HTTPAdapter
from requests.packages.urllib3.util.retry import Retry
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import MultipleLocator
from scipy.signal import argrelextrema
from datetime import datetime
from zoneinfo import ZoneInfo


def get_chinese_date():
    beijing_time = datetime.now(ZoneInfo("Asia/Shanghai"))

    year = beijing_time.year
    month = beijing_time.month 
    day = beijing_time.day  

    chinese_date = f"{year}年{month}月{day}日"
    return chinese_date

date = get_chinese_date()

def create_session():
    session = requests.Session()
    retry = Retry(
        total=3,
        backoff_factor=1,
        status_forcelist=[500, 502, 503, 504]
    )
    adapter = HTTPAdapter(max_retries=retry)
    session.mount('https://', adapter)
    return session

def parse_issue(issue):
    return int(issue[:4]), int(issue[4:])

def fetch_real_data(post_url):
    session = create_session()
    
    headers = {
        "Content-Type": "application/x-www-form-urlencoded; charset=UTF-8",
        "User-Agent": "desensitization",
        "Referer": "desensitization",
        "Cookie": "desensitization"
    }
    
    params = [
        f"client_type:0",
        f"lot_code:\"pl5\"",
        f"type1:\"xjbzs\"",
        f"list_num:20",
        f"start_issue:\"\"",
        f"last_issue:\"\"",
        f"filter:\"\""
    ]
    
    query = f'query{{get_zoushi_client_html({",".join(params)})}}'
    
    try:
        response = session.post(
            post_url,
            data={"query": query},
            headers=headers,
            timeout=30
        )
        response.raise_for_status()
        return response.text
    except Exception as e:
        print(f"请求失败: {str(e)}")
        return None

def parse_response_data(html):
    issue_pattern = re.compile(
        r'<td\s+issue=\\\"(\d{7})\\\"',  # patch issue
        re.IGNORECASE
    )
    issues = issue_pattern.findall(html)
    
    number_pattern = re.compile(
        r"<span\s.*?>\s*([\d\s]+?)\s*</span></td>",  # patch class
        re.DOTALL
    )
    numbers = number_pattern.findall(html)
    clean_numbers = []
    for number in numbers:
        clean_num = number.replace(" ", "")
        if len(clean_num) == 5 and clean_num.isdigit():
            clean_numbers.append((clean_num))
    
    if len(issues) != len(clean_numbers):
        print(f"数据不匹配警告: 期号数({len(issues)}) ≠ 号码数({len(numbers)})")
        min_length = min(len(issues), len(numbers))
        issues = issues[:min_length]
        numbers = numbers[:min_length]
    
    cleaned = {}
    for issue, num_str in zip(issues, clean_numbers):
        if not re.match(r'^\d{7}$', issue):
            continue
        
        cleaned[issue] = num_str
        # cleaned.append((issue, num_str))
    
    return cleaned


def get_data():
    get_url = "desensitization"
    post_url = "desensitization"
    
    try:
        if os.path.exists('data.json'):
            with open('data.json', 'r', encoding='utf-8') as f:
                existing_data = json.load(f)
                if isinstance(existing_data, list):
                    existing_data = {item[0]: item[1] for item in existing_data}
        else:
            existing_data = OrderedDict()

        session = create_session()
        response = session.get(get_url)

        result = {}
        html = fetch_real_data(post_url)
        new_data = parse_response_data(html) if html else {}
        # print(f"{new_data}")

        actual_new = {k: v for k, v in new_data.items() if k not in existing_data}
        
        if actual_new:
            merged_data = OrderedDict(list(existing_data.items()) + list(actual_new.items()))
            
            if len(merged_data) > 1000:
                to_remove = len(merged_data) - 1000
                for _ in range(to_remove):
                    merged_data.popitem(last=False)
            
            sorted_data = OrderedDict(
                sorted(merged_data.items(), key=lambda x: x[0])
            )
            
            with open('data.json', 'w', encoding='utf-8') as f:
                json.dump(sorted_data, f, ensure_ascii=False, indent=2)
                
            print(f"发现新数据，数据自动更新完成。实际新增 {len(actual_new)} 条数据")
        else:
            print("没有需要更新的数据")

            
    except Exception as e:
        print(f"程序终止: {str(e)}")

def validate_input(mode, user_input):
    if not user_input.isdigit():
        return False, "输入必须为纯数字"
    
    length = len(user_input)
    if mode == 2:
        return (2 <= length <=5), "二位模式需要2-5位数字"
    elif mode == 3:
        return (3 <= length <=5), "三位模式需要3-5位数字"
    return False, "未知模式"

def two_digit_mode():
    print("\n查询二位遗漏值（2-5位数字）")
    while True:
        try:
            inp = input("请输入数字（q返回）> ").strip()
            if inp.lower() == 'q':
                return
            
            valid, msg = validate_input(2, inp)
            if valid:
                current = get_input_combinations(inp)
                for i in current:
                    plot_missing_trend(i, 'missing_results.json', inp)
                print(f"【执行成功】 折线图已保存在文件夹[{inp}]中")
            else:
                print(f"❌ 错误: {msg}")
                
        except KeyboardInterrupt:
            print("\n操作已取消")
            return

def three_digit_mode():
    print("\n查询三位遗漏值（3-5位数字）")
    while True:
        try:
            inp = input("请输入数字（q返回）> ").strip()
            if inp.lower() == 'q':
                return
            
            valid, msg = validate_input(3, inp)
            if valid:
                current = get_input_three_combinations(inp)
                for i in current:
                    plot_three_missing_trend(i, 'missing_results.json', inp)
                print(f"【执行成功】 折线图已保存在文件夹[{inp}]中")
            else:
                print(f"❌ 错误: {msg}")
                
        except KeyboardInterrupt:
            print("\n操作已取消")
            return

def main_menu():
    menu = """
=== 主菜单 ===
1) 计算二位遗漏值
2) 计算三位遗漏值
3) 退出
请选择操作: """
    
    while True:
        try:
            choice = input(menu).strip()
            if choice == '1':
                two_digit_mode()
            elif choice == '2':
                three_digit_mode()
            elif choice in ('3', 'q', 'exit'):
                print("感谢使用！")
                sys.exit(0)
            else:
                print("无效选项，请重新输入")
                
        except KeyboardInterrupt:
            print("\n检测到退出请求...")
            sys.exit(0)



def generate_all_combinations():
    combinations = set()
    for a, b in itertools.product('0123456789', repeat=2):
        sorted_combo = ''.join(sorted([a, b]))
        combinations.add(sorted_combo)
    return sorted(combinations)

def get_current_combinations(number_str):
    digits = list(number_str)
    current = set()
    for i, j in itertools.combinations(range(5), 2):
        d1, d2 = digits[i], digits[j]
        sorted_combo = ''.join(sorted([d1, d2]))
        current.add(sorted_combo)
    return current

def get_current_three_combinations(number_str):
    digits = list(number_str)
    current = set()
    for i, j, k in itertools.combinations(range(5), 3):
        d1, d2, d3 = digits[i], digits[j], digits[k]
        sorted_combo = ''.join(sorted([d1, d2, d3]))
        current.add(sorted_combo)
    return current

def generate_all_three_combinations():
    combinations = set()
    for a, b, c in itertools.product('0123456789', repeat=3):
        sorted_combo = ''.join(sorted([a, b, c]))
        combinations.add(sorted_combo)
    return sorted(combinations)

def calculate_missing_values(data_path):
    with open(data_path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    all_combos = generate_all_combinations()
    miss_count = defaultdict(int)
    for combo in all_combos:
        miss_count[combo] = 0
    
    sorted_items = sorted(data.items(), key=lambda x: x[0])
    
    history = []
    
    for issue, number_str in sorted_items:
        current_combos = get_current_combinations(number_str)
        
        for combo in all_combos:
            miss_count[combo] += 1
        
        for combo in current_combos:
            miss_count[combo] = 0
        
        history.append({
            'issue': issue,
            'current_missing': dict(miss_count)
        })
    
    three_all_combos = generate_all_three_combinations()
    three_miss_count = defaultdict(int)
    for combo in three_all_combos:
        three_miss_count[combo] = 0
    
    three_sorted_items = sorted(data.items(), key=lambda x: x[0])
    
    three_history = []
    
    for issue, number_str in three_sorted_items:
        three_current_combos = get_current_three_combinations(number_str)
        
        for combo in three_all_combos:
            three_miss_count[combo] += 1
        
        for combo in three_current_combos:
            three_miss_count[combo] = 0
        
        three_history.append({
            'issue': issue,
            'current_missing': dict(three_miss_count)
        })
    with open('missing_results.json', 'w') as f:
        json.dump({
            'last_missing': dict(miss_count),
            'max_missing': {k: max(v for v in [h['current_missing'][k] for h in history]) for k in all_combos},
            'history': history,
            'last_three_missing': dict(three_miss_count),
            'max_three_missing': {k: max(v for v in [h['current_missing'][k] for h in three_history]) for k in three_all_combos},
            'three_history': three_history,
        }, f, indent=2)

def get_input_combinations(number_str):
    digits = list(number_str)
    current = set()
    for i, j in itertools.combinations(range(len(digits)), 2):
        d1, d2 = digits[i], digits[j]
        sorted_combo = ''.join(sorted([d1, d2]))
        current.add(sorted_combo)
    return current

def get_input_three_combinations(number_str):
    digits = list(number_str)
    current = set()
    for i, j, k in itertools.combinations(range(len(digits)), 3):
        d1, d2, d3 = digits[i], digits[j], digits[k]
        sorted_combo = ''.join(sorted([d1, d2, d3]))
        current.add(sorted_combo)
    return current

def plot_missing_trend(key_str, json_file_path, input_str):
    with open(json_file_path, 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    history = data.get('history', [])[-300:]
    y_values = [entry['current_missing'].get(key_str, 0) for entry in history]
    
    x_values = np.arange(1, len(y_values)+1)
    y_values = np.array(y_values)

    fig, ax = plt.subplots(figsize=(24, 6))  

    peak_indices = []
    current_max = None
    current_max_idx = None
    zero_count = 0

    for idx, val in enumerate(y_values):
        if val == 0:
            if current_max is not None:
                peak_indices.append(current_max_idx)
                current_max = None
            zero_count += 1
        else:
            if zero_count >= 2:
                peak_indices.append(idx - 1)  
            zero_count = 0
            
            if current_max is None or val >= current_max:
                current_max = val
                current_max_idx = idx

    if current_max is not None:
        peak_indices.append(current_max_idx)

    peak_indices = np.array(peak_indices)
    
    ax.plot(x_values[peak_indices], y_values[peak_indices],
            linestyle='-', 
            color='#1f77b4',
            alpha=0.5,
            linewidth=0.6,
            )

    ax.yaxis.set_major_locator(MultipleLocator(1))
    ax.grid(True, 
            axis='y',  
            linestyle=':', 
            color='lightgray', 
            linewidth=0.8)

    for idx in peak_indices:
        x, y = x_values[idx], y_values[idx]
        
        ax.annotate(
            f"{y}",
            (x, y),
            xytext=(0, 8 if y > 0 else -8),
            textcoords='offset points',
            ha='center',
            va='bottom' if y > 0 else 'top',
            color='darkred',
            fontsize=10,
            bbox=dict(boxstyle='round,pad=0.2', fc='white', ec='none', alpha=0.8),
            arrowprops=dict(arrowstyle='->', color='gray', lw=0.5) if y > 0 else None
        )
        ax.plot(x, y, 'o', markersize=3, color='crimson', alpha=0.9)

    last_x = x_values[-1]
    last_y = y_values[-1]

    ax.annotate(
        f"{last_y}",
        (last_x, last_y),
        xytext=(0, 8),
        textcoords='offset points',
        ha='center',
        va='bottom',
        color='darkred',
        fontsize=10,
        bbox=dict(boxstyle='round,pad=0.2', fc='white', ec='none', alpha=0.8),
        arrowprops=dict(arrowstyle='->', color='gray', lw=0.5)
    )
    ax.plot(last_x, last_y, 'o', markersize=3, color='crimson', alpha=0.9)

    max_x = len(x_values)
    x_step = max(1, int(max_x / 50))  
    ax.set_xticks(np.arange(0, max_x+1, x_step))
    plt.setp(ax.get_xticklabels(), 
            rotation=30, 
            ha='right',
            fontsize=8)

    ax.set_xlim(0, max_x+1)
    ax.set_ylim(-0.5, np.max(y_values)+1)

    ax.set_title(f"{key_str}", 
                pad=20, 
                fontsize=16,
                fontweight='bold')

    plt.tight_layout()
    plt.rcParams['font.family'] = 'SimHei'
    os.makedirs(input_str, exist_ok=True)
    plt.savefig(f'{input_str}/遗漏值{key_str}（{date}）.png', dpi=300)

def plot_three_missing_trend(key_str, json_file_path, input_str):
    with open(json_file_path, 'r', encoding='utf-8') as file:
        data = json.load(file)
    
    history = data.get('three_history', [])[-600:]
    y_values = [entry['current_missing'].get(key_str, 0) for entry in history]
    
    x_values = np.arange(1, len(y_values)+1)
    y_values = np.array(y_values)

    fig, ax = plt.subplots(figsize=(24, 6))  

    peak_indices = []
    current_max = None
    current_max_idx = None
    zero_count = 0

    for idx, val in enumerate(y_values):
        if val == 0:
            if current_max is not None:
                peak_indices.append(current_max_idx)
                current_max = None
            zero_count += 1
        else:
            if zero_count >= 2:
                peak_indices.append(idx - 1)  
            zero_count = 0
            
            if current_max is None or val >= current_max:
                current_max = val
                current_max_idx = idx

    if current_max is not None:
        peak_indices.append(current_max_idx)

    peak_indices = np.array(peak_indices)
    
    ax.plot(x_values[peak_indices], y_values[peak_indices], 
            linestyle='-', 
            color='#1f77b4',
            alpha=0.5,
            linewidth=0.6)

    ax.yaxis.set_major_locator(MultipleLocator(5))  
    ax.grid(True, 
            axis='y',  
            linestyle=':', 
            color='lightgray', 
            linewidth=0.8)

    for idx in peak_indices:
        x, y = x_values[idx], y_values[idx]
        ax.annotate(
            f"{y}",
            (x, y),
            xytext=(0, 8),
            textcoords='offset points',
            ha='center',
            va='bottom',
            color='darkred',
            fontsize=10,
            bbox=dict(boxstyle='round,pad=0.2', fc='white', ec='none', alpha=0.8),
            arrowprops=dict(arrowstyle='->', color='gray', lw=0.5)
        )
        ax.plot(x, y, 'o', markersize=3, color='crimson', alpha=0.9)

    last_x = x_values[-1]
    last_y = y_values[-1]

    ax.annotate(
        f"{last_y}",
        (last_x, last_y),
        xytext=(0, 8),
        textcoords='offset points',
        ha='center',
        va='bottom',
        color='darkred',
        fontsize=10,
        bbox=dict(boxstyle='round,pad=0.2', fc='white', ec='none', alpha=0.8),
        arrowprops=dict(arrowstyle='->', color='gray', lw=0.5)
    )
    ax.plot(last_x, last_y, 'o', markersize=3, color='crimson', alpha=0.9)

    max_x = len(x_values)
    x_step = max(1, int(max_x / 50))  
    ax.set_xticks(np.arange(0, max_x+1, x_step))
    plt.setp(ax.get_xticklabels(), 
            rotation=30, 
            ha='right',
            fontsize=8)

    ax.set_xlim(0, max_x+1)
    ax.set_ylim(-0.5, np.max(y_values)+1)

    ax.set_title(f"{key_str}", 
                pad=20, 
                fontsize=16,
                fontweight='bold')
    # ax.set_xlabel("期号序列", fontsize=10)
    # ax.set_ylabel("遗漏值", fontsize=10)

    plt.tight_layout()
    plt.rcParams['font.family'] = 'SimHei'
    os.makedirs(input_str, exist_ok=True)
    plt.savefig(f'{input_str}/遗漏值{key_str}（{date}）.png', dpi=300)  

def banner():
    print('''
    o8o                        o8o                                                              
    `"'                        `"'                                                              
   oooo  .ooooo.  oooo  oooo  oooo   .oooo.o  .oooo.o  .oooo.   ooo. .oo.    .ooooo.   .ooooo.  
   `888 d88' `88b `888  `888  `888  d88(  "8 d88(  "8 `P  )88b  `888P"Y88b  d88' `"Y8 d88' `88b 
    888 888   888  888   888   888  `"Y88b.  `"Y88b.   .oP"888   888   888  888       888ooo888 
    888 888   888  888   888   888  o.  )88b o.  )88b d8(  888   888   888  888   .o8 888    .o 
    888 `Y8bod8P'  `V88V"V8P' o888o 8""888P' 8""888P' `Y888""8o o888o o888o `Y8bod8P' `Y8bod8P' 
    888                                                                                         
.o. 88P                                                                                         
`Y888P                                                                                              
    ''')

if __name__ == "__main__":
    banner()
    get_data()
    calculate_missing_values('data.json')
    main_menu()
