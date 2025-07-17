import sys
import csv
import matplotlib.pyplot as plt
import numpy as np



def read_csv(filename):

    try:
        with open(filename, 'r', encoding='utf-8') as file:
            reader = csv.DictReader(file)
            data = list(reader)
        return data
    except FileNotFoundError:
        print("Error file not found !")
        sys.exit(1)
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)


def is_numerical_column(data, column_name, threshold=0.8):

    if not data:
        return False
    
    numerical_count = 0
    total_valid_count = 0 
    
    #check first 50 data
    sample_size = min(len(data), 50)
    
    for i in range(sample_size):
        value = data[i].get(column_name, '')
        
        # skip null, nan, empty
        if value is None or value == '' or str(value).lower() in ['nan', 'null', 'none']:
            continue
        
        total_valid_count += 1
        
        # check float
        try:
            float(value)
            numerical_count += 1
        except (ValueError, TypeError):
            # not a number, pass
            pass
    

    if total_valid_count == 0:
        return False
    
    # check percentage of number
    numerical_percentage = numerical_count / total_valid_count
    return numerical_percentage >= threshold



def check_identifier(data, column_name):
    #check identifier column
    column_lower = column_name.lower()
    identifier_keywords = ['index', 'id', 'key', 'number', 'num', 'seq']
    
    if any(keyword in column_lower for keyword in identifier_keywords):
        return True
    
    
    sample_size = min(len(data), 10)
    try:
        values = []
        for i in range(sample_size):
            value = data[i].get(column_name, '')
            if value != '' and value is not None:
                values.append(float(value))
        
        if len(values) >= 3:
            
            differences = [values[i+1] - values[i] for i in range(len(values)-1)]
            if all(diff == 1 for diff in differences):
                return True
    except (ValueError, TypeError):
        pass
    
    return False





def get_numerical_columns(data):
    #detect numerical column
    if not data:
        return []
    
    all_columns = list(data[0].keys())
    numerical_columns = []
    
    for column in all_columns:
        # Check if contains numerical data
        if is_numerical_column(data, column):
            # skip other identifier
            if not check_identifier(data, column):
                numerical_columns.append(column)
    
    return numerical_columns


def house_sort(data):

    #group data by house

    house_data = {} #dictionary for sorted data by house and by course
    numerical_columns = get_numerical_columns(data) #list of course

    for row in data:
        house = (row.get('Hogwarts House', '') or '').strip()
        if not house:
            continue

        if house not in house_data:
            house_data[house] = {}
            for col in numerical_columns:
                house_data[house][col] = []

    #add value for each course of each house
        for col in numerical_columns:
            value = row.get(col, '')
            if value and value not in ['', 'nan', 'null', None]:
                try:
                    house_data[house][col].append(float(value))
                except (ValueError, TypeError):
                    continue

    return house_data


def calculate_mean(values):

    if not values:
        return 0
    return sum(values) / len(values)


def calculate_std(values):

    if len(values) <= 1:
        return 0
    
    mean = calculate_mean(values)
    variance = sum((x - mean) ** 2 for x in values) / (len(values) - 1)
    return variance ** 0.5




def calculate_homogeneity_score(house_data, course):


    #calculate homogen score for all houses
    #coefficient of variations
    house_stds = []
    
    for house in house_data:
        if course in house_data[house] and house_data[house][course]:
            std = calculate_std(house_data[house][course])
            house_stds.append(std)
    
    if len(house_stds) < 2:
        return float('inf')
    

    mean_std = calculate_mean(house_stds)
    if mean_std == 0:
        return 0 
    
    std_of_stds = calculate_std(house_stds)
    return std_of_stds / mean_std






def find_most_homogen(house_data):

    #find most homogeneous course across houses
    all_course = set() #build collection
    for house in house_data.values():
        all_course.update(house.keys())

    course_scores = {}

    for course in all_course:
        score = calculate_homogeneity_score(house_data, course)
        course_scores[course] = score


    most_homogeneous_course = None
    min_score = float('inf')

    for course, score in course_scores.items():
        if score < min_score:
            min_score = score
            most_homogeneous_course = course
    
    return most_homogeneous_course, course_scores


def manual_sort_by_score(course_scores):

    items = list(course_scores.items())
    n = len(items)
    
    #Bubble sort
    for i in range(n):
        for j in range(0, n - i - 1):
            if items[j][1] > items[j + 1][1]:  # Compare scores
                items[j], items[j + 1] = items[j + 1], items[j]
    
    return items




def display_homogen_score(course_scores):


    print("HOMOGENEITY SCORES FOR ALL COURSES")
    print("(Lower score = more homogeneous across houses)")
    print("-"*60)
    
    #sort course by homogen score
    sorted_courses = manual_sort_by_score(course_scores)
    
    for i, (course, score) in enumerate(sorted_courses, 1):
        if score == float('inf'):
            print(f"{i:2d}. {course:<25} : No sufficient data")
        else:
            print(f"{i:2d}. {course:<25} : {score:.4f}")
    

    most_homogeneous = sorted_courses[0][0] if sorted_courses else "None"
    print(f"Most homogeneous course: {most_homogeneous}")
    print("-"*60)


def find_min_max(values):
    
    if not values:
        return None, None
    
    min_val = values[0]
    max_val = values[0]
    
    for val in values:
        if val < min_val:
            min_val = val
        if val > max_val:
            max_val = val
    
    return min_val, max_val



def display_histogram(house_data, course_scores):
    #display interactive choice histogram view
    courses = list(course_scores.keys())
    
    while True:
        print("HISTOGRAM DISPLAY")
        print("Available courses:")
        
        for i, course in enumerate(courses, 1):
            score = course_scores[course]
            score_str = f"{score:.4f}" if score != float('inf') else "No data"
            print(f"{i:2d}. {course} (Homogeneity: {score_str})")
        
        print(f"{len(courses) + 1:2d}. Exit")
        print("-"*50)
        
        try:
            choice = input("Choose a course number to view its histogram: ").strip()
            
            if choice == str(len(courses) + 1) or choice.lower() in ['exit', 'quit', 'q']:
                break
            
            course_index = int(choice) - 1
            if 0 <= course_index < len(courses):
                selected_course = courses[course_index]
                create_histogram(house_data, selected_course)
            else:
                print("Invalid choice. Please try again.")
        
        except ValueError:
            print("Please enter a valid number.")
        except KeyboardInterrupt:
            break



def create_histogram(house_data, course):
    #create histogram for the chosen course
    plt.figure(figsize=(12, 8))
    
    houses = list(house_data.keys())
    colors = ["#0044FF", "#064900", "#F70000", "#EEFF00"] #Raven, Sly, Grif, Huffle
    
    #list of all score for the selected course across all house
    all_values = []
    for house in houses:
        if course in house_data[house]:
            all_values.extend(house_data[house][course])
    
    if not all_values:
        print(f"No data available for course: {course}")
        return
    
    min_val, max_val = find_min_max(all_values)
    

    num_bins = 20
    bin_width = (max_val - min_val) / num_bins
    bins = [min_val + i * bin_width for i in range(num_bins + 1)]
    
    for i, house in enumerate(houses): #get index and house name
        if course in house_data[house] and house_data[house][course]:
            values = house_data[house][course] #score for this house in this course
            plt.hist(values, bins=bins, alpha=0.2, label=house, 
                    color=colors[i % len(colors)], edgecolor='black', linewidth=0.5)
    
    plt.title(f'Score Distribution for {course}\nAcross Hogwarts Houses', 
              fontsize=16, fontweight='bold', pad=20)
    plt.xlabel('Score', fontsize=12)
    plt.ylabel('Frequency', fontsize=12)
    plt.legend(title='Houses', fontsize=10)
    plt.grid(True, alpha=0.3)
    
    # Add statistics
    stats_text = f"Total students: {len(all_values)}\nScore range: {min_val:.2f} - {max_val:.2f}"
    plt.text(0.02, 0.98, stats_text, transform=plt.gca().transAxes, 
             verticalalignment='top', bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
    
    plt.tight_layout()
    plt.show()


def main():

    if len(sys.argv) != 2:
        print("Error ! (python histogram.py dataset.csv)")
        sys.exit(1)

    filename = sys.argv[1]

    data = read_csv(filename)
    data_sort_house = house_sort(data)

    #calculate homogeneity
    most_homogen, homogen_score = find_most_homogen(data_sort_house)

    display_homogen_score(homogen_score)

    display_histogram(data_sort_house, homogen_score)



if __name__ == "__main__":
    main()