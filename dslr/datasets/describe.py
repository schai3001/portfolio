import sys
import csv



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



def calculate_sum(values):
    #calculate sum
    if not values:
        return 0
    
    total = 0
    for value in values:
        total += value
    
    return total



def calculate_count(values):
    #calculate number of values
    return len(values)


def calculate_mean(values):

    if not values:
        return float('nan')
    
    total = calculate_sum(values)
    return total / len(values)


def calculate_std(values):
    #calculate std
    if len(values) < 2:
        return float('nan')
    
    mean = calculate_mean(values)
    
    #calculate sum suare diff
    sum_squared_tot = 0
    for value in values:
        difference = value - mean
        sum_squared_tot += difference * difference
    
    variance = sum_squared_tot / (len(values) - 1)
    return variance ** 0.5


def calculate_min(values):
    #find min
    if not values:
        return float('nan')
    
    minimum = values[0]
    

    for value in values[1:]: #start from index 1
        if value < minimum:
            minimum = value
    
    return minimum


def calculate_max(values):

    if not values:
        return float('nan')
    

    maximum = values[0]

    for value in values[1:]:
        if value > maximum:
            maximum = value
    
    return maximum


def sort_values(values):

    #sort insertion algorithm
    if not values:
        return []
    
    #create a copy list
    sorted_values = values[:]

    for i in range(1, len(sorted_values)):
        key = sorted_values[i]
        j = i - 1

        #move elem to one position
        while j >= 0 and sorted_values[j] > key:
            sorted_values[j + 1] = sorted_values[j]
            j -= 1

        # place key
        sorted_values[j + 1] = key

    return sorted_values



def calculate_percentile(values, percentile):
    #calculate percentile (0-100)
    if not values:
        return float('nan')
    
    sorted_values = sort_values(values)
    n = len(sorted_values)

    if percentile == 0:
        return sorted_values[0]
    if percentile == 100:
        return sorted_values[-1]
    
    rank = (percentile / 100) * (n - 1)

    if rank == int(rank): #check rank whole number
        return sorted_values[int(rank)]
    else:
        # linear interpolation
        lower = int(rank)
        upper = lower + 1
        weight = rank - lower

        if upper >= n:
            return sorted_values[-1]
        
        return sorted_values[lower] * (1 - weight) + sorted_values[upper] * weight



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




def describe_dataset(filename):

    #main function
    data = read_csv(filename) #read csv file and return list data

    if not data:
        print("Error no data found !")
        return

    
    check_columns = get_numerical_columns(data)

    if not check_columns:
        print("Error no numerical column !")
        return


    #calculate stat for each column
    stats = {}
    for column in check_columns:
        values = extract_numerical_data(data, column)

        stats[column] = {
            'Count': calculate_count(values),
            'Mean': calculate_mean(values),
            'Std': calculate_std(values),
            'Min': calculate_min(values),
            '25%': calculate_percentile(values, 25),
            '50%': calculate_percentile(values, 50),
            '75%': calculate_percentile(values, 75),
            'Max': calculate_max(values)
        }

    print_stat(stats, check_columns)



def print_stat(stats, columns):

    
    if not columns:
        print("No columns to display !")
        return
    
    row_names = ['Count', 'Mean', 'Std', 'Min', '25%', '50%', '75%', 'Max']

    for i, column in enumerate(columns, 1):
        print(f"\n{i}. {column}")
        print("----------------------------------")
        
        for stat in row_names:
            value = stats[column][stat]
            if stat == 'Count':
                print(f"{stat}: {int(value)}")
            else:
                if value != value:  # NaN check
                    print(f"{stat}: NaN")
                else:
                    print(f"{stat}: {value:.6f}")
    


def main():
    #check argv
    if len(sys.argv) != 2:
        print("Error ! (describe.py <dataset.csv>)")
        sys.exit(1)
    
    filename = sys.argv[1]
    describe_dataset(filename)



if __name__ == "__main__":
    main()
