import csv
import json
import matplotlib.pyplot as plt
import numpy as np



def load_data(filename):

    #load data from data.csv
    mileages = []
    prices = []

    try:
        with open(filename, 'r') as file:
            reader = csv.DictReader(file)
            for row in reader:
                mileages.append(float(row['km']))
                prices.append(float(row['price']))

        print(f"Loaded {len(mileages)} data from {filename} !")
        return mileages, prices
    
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
        return None, None
    except Exception as e:
        print(f"Error loading data: {e}")
        return None, None



def normalize_data(data):

    #normalize data to improve gradient descent

    data_array = np.array(data)
    mean = np.mean(data_array)
    std = np.std(data_array)
    normalized = (data_array - mean) / std if std != 0 else data_array - mean
    return normalized.tolist(), mean, std


def denormalize(norm_theta0, norm_theta1, mileage_mean, mileage_std, price_mean, price_std):

    denorm_theta1 = norm_theta1 * (price_std / mileage_std)
    denorm_theta0 = price_mean + price_std * norm_theta0 - denorm_theta1 * mileage_mean
    return denorm_theta0, denorm_theta1



def estimate_price(mileage, theta0, theta1):
    return theta0 + (theta1 * mileage)


def calculate_cost(mileages, prices, theta0, theta1):

    m = len(mileages)
    total_cost = 0
    
    for i in range(m):
        prediction = estimate_price(mileages[i], theta0, theta1)
        error = prediction - prices[i]
        total_cost += error * error
    
    return total_cost / (2 * m)



def save_theta(theta0, theta1):

    #save theta in a file for program 1
    data = {
        'theta0': theta0,
        'theta1': theta1
    }


    try:
        with open('theta_data.json', 'w') as f:
            json.dump(data, f)
        print(f"Theta data saved in theta_data.json file !")

    except Exception as e:
        print(f"Error : {e}")



def gradient_descent(mileages, prices, learning_rate = 0.1, iterations = 100):
    
    m = len(mileages)
    theta0 = 0.0
    theta1 = 0.0
    costs = []

    for iteration in range(iterations):

        sum_error = 0.0
        sum_error_x = 0.0

        for i in range(m):
            prediction = estimate_price(mileages[i], theta0, theta1)
            error = prediction - prices[i]
            sum_error += error
            sum_error_x += error * mileages[i]

        tmp_theta0 = learning_rate * (1/m) * sum_error
        tmp_theta1 = learning_rate * (1/m) * sum_error_x


        theta0 -= tmp_theta0
        theta1 -= tmp_theta1


        if iteration % 10 == 0:
            cost = calculate_cost(mileages, prices, theta0, theta1)
            costs.append(cost)
            print(f"Iteration {iteration:4d}: Cost = {cost:.2f}, θ0 = {theta0:.6f}, θ1 = {theta1:.6f}")


    final_cost = calculate_cost(mileages, prices, theta0, theta1)
    costs.append(final_cost)

    print(f"Final Cost = {final_cost:.2f}, θ0 = {theta0:.6f}, θ1 = {theta1:.6f}")

    return theta0, theta1, costs



def plot(mileages, prices, theta0, theta1, costs):

    try:
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))



        #plot 1 price vs mileage
        ax1.scatter(mileages, prices, color='blue', alpha=0.7, label='Data points')

        min_mileage = min(mileages)
        max_mileage = max(mileages)
        line_x = [min_mileage, max_mileage]
        line_y = [estimate_price(x, theta0, theta1) for x in line_x]

        ax1.plot(line_x, line_y, color='red', linewidth=2, label='Fitted line')
        ax1.set_xlabel('Mileage (km)')
        ax1.set_ylabel('Price ($)')
        ax1.set_title('Car Price vs Mileage')
        ax1.legend()
        ax1.grid(True, alpha=0.3)

        #plot2 cost during training

        ax2.plot(range(0, len(costs) * 10, 10), costs, color='green', linewidth=2)
        ax2.set_xlabel('Iterations')
        ax2.set_ylabel('Cost')
        ax2.set_title('Cost During Training')
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('plot_results.png', dpi=300, bbox_inches='tight')
        plt.show()

    except Exception as e:
        print(f"Error plot: {e}")


def main():

    #load data (mileage and price)
    mileages, prices = load_data('data.csv')
    if mileages is None:
        return

    print(f"Mileage: {min(mileages):.0f} to {max(mileages):.0f} km")
    print(f"Price: ${min(prices):.0f} to ${max(prices):.0f}")


    #normalize data for better gradient descent
    #scale problem

    norm_mileages, mileage_mean, mileage_std = normalize_data(mileages)
    norm_prices, price_mean, price_std = normalize_data(prices)

    #print(f"Norm mileages : {norm_mileages}")
    #print(f"Norm prices : {norm_prices}")


    # gradient descent with normalized data

    norm_theta0, norm_theta1, costs = gradient_descent(
        norm_mileages, 
        norm_prices,
        learning_rate = 0.1,
        iterations = 100
    )



    theta0, theta1 = denormalize(
        norm_theta0, norm_theta1, mileage_mean, 
        mileage_std, price_mean, price_std
    )

    print(f"final θ0 = {theta0:.6f}")
    print(f"final θ1 = {theta1:.6f}")


    save_theta(theta0, theta1)


    # Test 
    test_mileages = [50000, 100000, 150000, 200000]
    for test_mileage in test_mileages:
        predicted_price = estimate_price(test_mileage, theta0, theta1)
        print(f"Mileage: {test_mileage:6d} km --> Predicted price: ${predicted_price:7.2f}")


    r_mean = sum(prices) / len(prices)
    ss_tot = sum((y - r_mean) ** 2 for y in prices)
    ss_res = sum((prices[i] - estimate_price(mileages[i], theta0, theta1)) ** 2 for i in range(len(prices)))
    r_squared = 1 - (ss_res / ss_tot) if ss_tot != 0 else 0
    print(f"R-squared : {r_squared:.4f}")


    #Graph results

    plot(mileages, prices, theta0, theta1, costs)




if __name__ == "__main__":
    main()