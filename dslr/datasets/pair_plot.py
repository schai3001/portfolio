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



def get_course_with_houses(data, course):
    values = []
    houses = []
    
    for row in data:
        try:
            value = row.get(course, '')
            house = row.get('Hogwarts House', '').strip()
            
            if (value and value not in ['', 'nan', 'null', None] and house):
                values.append(float(value))
                houses.append(house)
                
        except (ValueError, TypeError):
            continue
    
    return values, houses



def histogram(ax, course, data):

    values, houses = get_course_with_houses(data, course)

    if not values:
        ax.text(0.5, 0.5, 'No Data', transform=ax.transAxes, 
                ha='center', va='center', fontsize=8)
        return

    house_colors = {
        'Ravenclaw': '#0044FF',
        'Slytherin': '#064900', 
        'Gryffindor': '#F70000',
        'Hufflepuff': '#EEFF00'
    }

    house_data = {house: [] for house in house_colors.keys()}
    for i, house in enumerate(houses):
        if house in house_data:
            house_data[house].append(values[i])


    all_values = values
    if all_values:
        min_val = min(all_values)
        max_val = max(all_values)


        num_bins = 20 
        if max_val != min_val:
            bin_width = (max_val - min_val) / num_bins
            bins = [min_val + i * bin_width for i in range(num_bins + 1)]
        else:
            bins = [min_val - 0.5, min_val + 0.5]
        
        for house, color in house_colors.items():
            if house_data[house]:
                ax.hist(house_data[house], bins=bins, alpha=0.6, 
                       color=color, density=True, 
                       edgecolor='black', linewidth=0.2)

    ax.set_title(course, fontsize=6, fontweight='bold')
    ax.tick_params(labelsize=4)
    ax.grid(True, alpha=0.2)

    ax.set_ylabel('')



def get_paired_values_with_houses(data, course1, course2):

    x_values = []
    y_values = []
    houses = []
    
    for row in data:
        try:
            value1 = row.get(course1, '')
            value2 = row.get(course2, '')
            house = row.get('Hogwarts House', '').strip()
            
            if (value1 and value1 not in ['', 'nan', 'null', None] and
                value2 and value2 not in ['', 'nan', 'null', None] and
                house):
                
                x_values.append(float(value1))
                y_values.append(float(value2))
                houses.append(house)
                
        except (ValueError, TypeError):
            continue
    
    return x_values, y_values, houses




def scatter_plot(ax, course_x, course_y, data, label_size=6):

    x_values, y_values, houses = get_paired_values_with_houses(data, course_x, course_y)
    
    if len(x_values) < 5:
        ax.text(0.5, 0.5, 'Insufficient\nData', transform=ax.transAxes, 
                ha='center', va='center', fontsize=6)
        return
    

    house_colors = {
        'Ravenclaw': '#0044FF',
        'Slytherin': '#064900', 
        'Gryffindor': '#F70000',
        'Hufflepuff': '#EEFF00'
    }
    

    for house in house_colors.keys():
        house_x = []
        house_y = []
        
        for i in range(len(houses)):
            if houses[i] == house:
                house_x.append(x_values[i])
                house_y.append(y_values[i])
        
        if house_x:  
            ax.scatter(house_x, house_y, 
                      c=house_colors[house], 
                      alpha=0.7, 
                      s=2,  
                      edgecolors='none')
    

    ax.tick_params(labelsize=label_size)
    ax.grid(True, alpha=0.2) 




def pair_plot(data):

    #get num columns
    courses = get_numerical_columns(data)

    if len(courses) < 2:
        print("Error: need more data !")
        return

    print(f"Total courses : {len(courses)}")
    for i, course in enumerate(courses, 1):
        print(f"{i:2d}. {course}")

    n_courses = len(courses)

    house_colors = {
        'Ravenclaw': '#0044FF',
        'Slytherin': '#064900', 
        'Gryffindor': '#F70000',
        'Hufflepuff': '#EEFF00'
    }

    fig, axes = plt.subplots(n_courses, n_courses, figsize=(20, 20))
    fig.suptitle(f'Pair Plot : All {n_courses} Hogwarts courses', fontsize=16, fontweight='bold')

    for i in range(n_courses):
        for j in range(n_courses):
            ax = axes[i, j]

            if i == j:
                histogram(ax, courses[i], data)

            else:
                scatter_plot(ax, courses[j], courses[i], data, label_size=4)

            if i == n_courses - 1:
                ax.set_xlabel(courses[j], fontsize=6, rotation=45)
            else:
                ax.set_xlabel('')

            if j == 0:
                ax.set_ylabel(courses[i], fontsize=6)
            else:
                ax.set_ylabel('')

            ax.tick_params(labelsize=4)

    legend_elements = []
    for house, color in house_colors.items():
        legend_elements.append(plt.Line2D([0], [0], marker='o', color='w', 
                                        markerfacecolor=color, markersize=6, 
                                        label=house, markeredgecolor='black'))

    fig.legend(handles=legend_elements, title='Hogwarts Houses', 
              loc='center right', fontsize=8)
    
    plt.subplots_adjust(left=0.05, bottom=0.05, right=0.95, top=0.93, 
                       wspace=0.3, hspace=0.3)

    plt.show()
    input("Press Enter to close the plot and continue...")



def main():
    if len(sys.argv) != 2:
        print("Error ! (python pair_plot.py dataset.csv)")
        sys.exit(1)

    filename = sys.argv[1]
    
    data = read_csv(filename)
    
    if not data:
        print("Error: No data found!")
        return
    
    pair_plot(data)


if __name__ == "__main__":
    main()