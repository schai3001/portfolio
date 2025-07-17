import sys
import csv
import matplotlib.pyplot as plt


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


def extract_numerical_data(data, column):
    #extract num and filter nan values
    values = []
    for row in data:
        try:
            value = row[column]
            if value is not None and value != '' and value != 'nan':
                values.append(float(value))
        except (ValueError, TypeError):
            continue
    return values



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



def calculate_mean(values):
    if not values:
        return 0
    return sum(values) / len(values)



def get_paired_values(data, course1, course2):
    
    #get paired list of values , filter no value row

    course1_values = []
    course2_values = []
    houses = []
    
    for row in data:
        try:
            value1 = row.get(course1, '')
            value2 = row.get(course2, '')
            house = row.get('Hogwarts House', '').strip()
            
            #skip if value missing for one the two
            if (value1 and value1 not in ['', 'nan', 'null', None] and
                value2 and value2 not in ['', 'nan', 'null', None] and
                house):
                
                course1_values.append(float(value1))
                course2_values.append(float(value2))
                houses.append(house)
                
        except (ValueError, TypeError):
            continue
    
    return course1_values, course2_values, houses



def calculate_corr(score_list1, score_list2):
    #calculate pearson correlation coeff for the pair

    if len(score_list1) != len(score_list2) or len(score_list1) < 2:
        return 0
    
    n = len(score_list1)


    mean1 = calculate_mean(score_list1)
    mean2 = calculate_mean(score_list2)

    num = 0
    sum_sq1 = 0
    sum_sq2 = 0

    for i in range(n):
        diff1 = score_list1[i] - mean1
        diff2 = score_list2[i] - mean2

        num += diff1 * diff2
        sum_sq1 += diff1 * diff1
        sum_sq2 += diff2 * diff2

    denom = (sum_sq1 * sum_sq2) ** 0.5

    if denom == 0:
        return 0
    
    return num / denom




def find_most_similar(data):

    numerical_columns = get_numerical_columns(data)

    if len(numerical_columns) < 2:
        print("Error: Need at least 2 numerical columns!")
        return None, None, 0
    
    max_corr = -1
    best_pair = (None, None)

    for i in range(len(numerical_columns)):
        for j in range(i + 1, len(numerical_columns)):
            course1 = numerical_columns[i]
            course2 = numerical_columns[j]

            score_list1, score_list2, houses = get_paired_values(data, course1, course2)

            if len(score_list1) >= 10: #need enough data
                corr = calculate_corr(score_list1, score_list2, )
                abs_corr = abs(corr)

                print(f"{course1:<25} vs {course2:<25} : {abs_corr:.4f}")

                if abs_corr > max_corr:
                    max_corr = abs_corr
                    best_pair = (course1, course2)

    if best_pair[0]:
        print(f"Most similar courses: {best_pair[0]} and {best_pair[1]}")
        print(f"Correlation coefficient: {max_corr:.4f}")
    
    return best_pair[0], best_pair[1], max_corr




def scatter_plot(data, course1, course2, max_corr):


    course1_values, course2_values, houses = get_paired_values(data, course1, course2)

    if len(course1_values) < 10:
        print(f"Not enough data points")
        return
    
    plt.figure(figsize=(12, 8))


    house_colors = {
        'Ravenclaw': '#0044FF',
        'Slytherin': '#064900',
        'Gryffindor': '#F70000',
        'Hufflepuff': '#EEFF00'
    }

    #points for each house
    for house in house_colors.keys():
        house_x = []
        house_y = []

        for i in range(len(houses)):
            if houses[i] == house:
                house_x.append(course1_values[i])
                house_y.append(course2_values[i])

        if house_x:
            plt.scatter(house_x, house_y, 
                       c=house_colors[house],
                       label=house, 
                       alpha=0.6, 
                       s=25,
                       edgecolors='black',
                       linewidth=0.5)

    plt.xlabel(course1, fontsize=12)
    plt.ylabel(course2, fontsize=12)
    plt.title(f'Scatter Plot: {course1} vs {course2}',
              fontsize=14, fontweight='bold', pad=20)

    plt.legend(title='Hogwarts Houses', fontsize=10)

    plt.grid(True, alpha=0.3)

    stats_text = f"Data points: {len(course1_values)}\nCorrelation: {max_corr:.4f}"
    plt.text(0.02, 0.98, stats_text, 
             transform=plt.gca().transAxes, 
             verticalalignment='top',
             bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
    
    
    plt.tight_layout()
    plt.show()


def main():
        
    if len(sys.argv) != 2:
        print("Error ! (python scatter_plot.py dataset.csv)")
        sys.exit(1)

    filename = sys.argv[1]
 
    data = read_csv(filename)

    if not data:
        print("Error : no data found !")
        return
    
    course1, course2, max_corr = find_most_similar(data)

    if not course1 or not course2:
        print("Error: Could not pair !")
        return


    scatter_plot(data, course1, course2, max_corr)



if __name__ == "__main__":
    main()