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


def load_model(filename='weights.json'):
    
    try:
        with open(filename, 'r') as f:
            model = json.load(f)
        
        print(f"Model loaded from {filename}")
        print(f"Features: {model['features']}")
        print(f"Houses: {model['houses']}")
        
        return model
    except FileNotFoundError:
        print(f"Error: Model file '{filename}' not found!")
        print("Make sure you run logreg_train.py first to train the model.")
        sys.exit(1)
    except Exception as e:
        print(f"Error loading model: {e}")
        sys.exit(1)


def prepare_test_data(data, feature_names):

    X_test = []
    indices = []
    
    for i, row in enumerate(data):

        feature_values = []
        valid_features = True
        
        for feature in feature_names:
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
            X_test.append(feature_values)
            indices.append(i)  
    
    X_test = np.array(X_test)
    
    print(f"Test data prepared: {X_test.shape[0]} students, {X_test.shape[1]} features")
    
    return X_test, indices


def normalize_test_data(X_test, means, stds):

    means = np.array(means)
    stds = np.array(stds)
    

    stds = np.where(stds == 0, 1, stds)
    
    X_normalized = (X_test - means) / stds
    
    return X_normalized


def sigmoid(z):

    z = np.clip(z, -500, 500)
    return 1 / (1 + np.exp(-z))


def predict_probabilities(X, classifiers):

    m = X.shape[0]
    

    X_with_bias = np.column_stack([np.ones(m), X])

    house_probabilities = {}
    
    for house, classifier in classifiers.items():
        theta = np.array(classifier['theta'])
        

        z = X_with_bias.dot(theta)
        probabilities = sigmoid(z)
        
        house_probabilities[house] = probabilities
    
    return house_probabilities


def make_predictions(house_probabilities):

    houses = list(house_probabilities.keys())
    m = len(house_probabilities[houses[0]])
    
    predictions = []
    prediction_probabilities = []
    
    for i in range(m):

        student_probs = {}
        for house in houses:
            student_probs[house] = house_probabilities[house][i]
        

        predicted_house = max(student_probs, key=student_probs.get)
        max_probability = student_probs[predicted_house]
        
        predictions.append(predicted_house)
        prediction_probabilities.append(student_probs)
    
    return predictions, prediction_probabilities


def save_predictions(predictions, indices, filename='houses.csv'):

    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        

        writer.writerow(['Index', 'Hogwarts House'])
        

        for i, (original_index, prediction) in enumerate(zip(indices, predictions)):
            writer.writerow([original_index, prediction])
    
    print(f"Predictions saved to {filename}")


def main():
    if len(sys.argv) < 2:
        print("Usage: python logreg_predict.py dataset_test.csv [weights.json]")
        sys.exit(1)
    
    test_filename = sys.argv[1]
    weights_filename = sys.argv[2] if len(sys.argv) > 2 else 'weights.json'
    


    model = load_model(weights_filename)
    


    test_data = read_csv(test_filename)
    

    X_test, indices = prepare_test_data(test_data, model['features'])
    
    if X_test.shape[0] == 0:
        print("Error: No valid test data found!")
        sys.exit(1)
    

    X_test_normalized = normalize_test_data(
        X_test, 
        model['normalization']['means'], 
        model['normalization']['stds']
    )
    

    house_probabilities = predict_probabilities(X_test_normalized, model['classifiers'])
    predictions, prediction_probabilities = make_predictions(house_probabilities)
    

    save_predictions(predictions, indices)


if __name__ == "__main__":
    main()