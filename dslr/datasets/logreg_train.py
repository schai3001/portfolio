import sys
import csv
import numpy as np
import json

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
    
    sample_size = min(len(data), 50)
    
    for i in range(sample_size):
        value = data[i].get(column_name, '')
        
        if value is None or value == '' or str(value).lower() in ['nan', 'null', 'none']:
            continue
        
        total_valid_count += 1
        
        try:
            float(value)
            numerical_count += 1
        except (ValueError, TypeError):
            pass
    
    if total_valid_count == 0:
        return False
    
    numerical_percentage = numerical_count / total_valid_count
    return numerical_percentage >= threshold


def check_identifier(data, column_name):
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
    if not data:
        return []
    
    all_columns = list(data[0].keys())
    numerical_columns = []
    
    for column in all_columns:
        if is_numerical_column(data, column):
            if not check_identifier(data, column):
                numerical_columns.append(column)
    
    return numerical_columns



def prepare_data(data, selected_features):

    print(f"Selected features: {selected_features}") 

    matrix_student_values = [] #matrice student x feature value
    list_houses = [] #list of houses

    for row in data:
        house = row.get('Hogwarts House', '').strip()
        if not house:
            continue

        feature_values = []
        valid_features = True

        for feature in selected_features:
            value = row.get(feature, '')
            try:
                if value and value not in ['', 'nan', 'null', None]:
                    feature_values.append(float(value))
                else:
                    valid_features = False
                    break
            except (ValueError, TypeError):
                valid_features = False
                break
        
        if valid_features:
            matrix_student_values.append(feature_values)
            list_houses.append(house)

    matrix_student_values = np.array(matrix_student_values)
    list_houses = np.array(list_houses)

    print(f"Dataset : {matrix_student_values.shape[0]} students, {matrix_student_values.shape[1]} features")


    unique_houses = list(set(list_houses))
    unique_houses.sort()
    print(f"Houses: {unique_houses}")
    
    return matrix_student_values, list_houses, selected_features, unique_houses


def calculate_mean(values):
    if not values:
        return 0
    
    total = 0
    count = 0
    
    for value in values:
        total += value
        count += 1
    
    return total / count


def calculate_std(values):

    if len(values) < 2:
        return 1
    
    mean = calculate_mean(values)
    
    sum_squared_diff = 0
    count = 0
    
    for value in values:
        diff = value - mean
        sum_squared_diff += diff * diff
        count += 1
    
    variance = sum_squared_diff / (count - 1)
    std = variance ** 0.5
    
    return std




def normalize_features(X):

    if not X or len(X) == 0:
        return X, [], []
    

    num_features = len(X[0])
    num_students = len(X)

    means = []
    stds = []


    for feature_index in range(num_features):

        feature_values = []
        for student_index in range(num_students):
            feature_values.append(X[student_index][feature_index])
        

        feature_mean = calculate_mean(feature_values)
        feature_std = calculate_std(feature_values)
        

        if feature_std == 0:
            feature_std = 1
        
        means.append(feature_mean)
        stds.append(feature_std)


    X_normalized = []

    for student_index in range(num_students):
        normalized_sample = []
        
        for feature_index in range(num_features):
            original_value = X[student_index][feature_index]
            normalized_value = (original_value - means[feature_index]) / stds[feature_index]
            normalized_sample.append(normalized_value)
        
        X_normalized.append(normalized_sample)
    
    X_normalized = np.array(X_normalized)
    means = np.array(means)
    stds = np.array(stds)
    
    return X_normalized, means, stds


def sigmoid(z):

    #extreme values to avoid overflow
    z = np.clip(z, -500, 500)
    return 1 / (1 + np.exp(-z))


def cost_function(X, y, theta):

    m = X.shape[0]
    

    z = X.dot(theta)
    h = sigmoid(z)
    

    epsilon = 1e-15
    h = np.clip(h, epsilon, 1 - epsilon)
    

    cost = -(1/m) * np.sum(y * np.log(h) + (1 - y) * np.log(1 - h))
    
    return cost



def gradient_descent(X, y, theta, learning_rate, num_iterations):

    m = X.shape[0]
    cost_history = []
    
    for i in range(num_iterations):

        z = X.dot(theta)
        h = sigmoid(z)
        

        cost = cost_function(X, y, theta)
        cost_history.append(cost)


        gradient = (1/m) * X.T.dot(h - y)
        

        theta -= learning_rate * gradient
    
    return theta, cost_history



def train(matrix, list_houses, houses, learning_rate=0.01, num_iterations=10000):

    print(f"Learning rate: {learning_rate}")
    print(f"Iterations: {num_iterations}")

    num_students, num_features = matrix.shape
    matrix_bias = np.column_stack((np.ones(num_students), matrix))  # add bias term
    
    #creates empty dictionary to store all 4 trained classifiers
    #each house will have its own classifier

    classifiers = {} 

    for house in houses:
        print(f"\n")
        print(f"Training {house} vs All")


        # 1 for current house, 0 for others
        y_binary = (list_houses == house).astype(int)

        #show number of students in the house and others
        print(f"Positive examples ({house}): {np.sum(y_binary)}")
        print(f"Negative examples (Others): {np.sum(1 - y_binary)}")

        theta = np.random.normal(0, 0.01, matrix_bias.shape[1])

        # train classifier with gradient descent
        theta_final, cost_history = gradient_descent(
            matrix_bias, y_binary, theta, learning_rate, num_iterations
        )

        #store classifier
        classifiers[house] = {
            'theta': theta_final.tolist(),
            'cost_history': cost_history
        }

        print(f"Final cost: {cost_history[-1]:.6f}")
        print(f"Parameters: {theta_final}")

    return classifiers


def save_model(classifiers, feature_names, normalization_params, filename='weights.json'):

    model = {
        'classifiers': classifiers,
        'features': feature_names,
        'normalization': {
            'means': normalization_params[0].tolist(),
            'stds': normalization_params[1].tolist()
        },
        'houses': list(classifiers.keys())
    }
    
    with open(filename, 'w') as f:
        json.dump(model, f, indent=2)
    
    print(f"\nModel saved to {filename}")
    
    

def main():

    if len(sys.argv) != 2:
        print("Usage: python logreg_train.py dataset_train.csv")
        sys.exit(1)


    filename = sys.argv[1]

    data = read_csv(filename)

    selected_features = ['Herbology', 'Astronomy', 'Defense Against the Dark Arts', 'Ancient Runes']

    matrix_student_values, list_houses, feature_names, houses = prepare_data(data, selected_features)

    if matrix_student_values.shape[0] == 0:
        print("Error: No valid training data found!")
        sys.exit(1)

    #normalize values
    matrix_normalized, means, stds = normalize_features(matrix_student_values)

    classifiers = train(matrix_normalized, list_houses, houses)

    save_model(classifiers, feature_names, (means, stds))


if __name__ == "__main__":
    main()