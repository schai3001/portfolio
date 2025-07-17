import os
import csv
import json


def load_trained_model():

    # useless here
    # load theta0 and theta1 from trained
    # if file not exist, default value 0, 0

    try:
        with open ('theta_data.json' , 'r') as file:
            data = json.load(file)
            theta0 = float(data['theta0'])
            theta1 = float(data['theta1'])
            print(f"theta0 = {theta0:.6f}")
            print(f"theta1 = {theta1:.6f}")
            return theta0, theta1, True

    except FileNotFoundError:
        print("No trained model found !")
        return 0.0, 0.0, False
    except Exception as e:
        print(f" Error loading model: {e}")
        return 0.0, 0.0, False



def load_dataset():

    # Load data
    # If not set 0,0

    try:
        mileages = []
        prices = []

        with open('data.csv', 'r') as file:
            csv_reader = csv.DictReader(file)
            # csv reader auto header first row

            for row in csv_reader:
                mileages.append(float(row['km']))
                prices.append(float(row['price']))
        
        print(f"Training data loaded !")
        return mileages, prices
    
    except FileNotFoundError:
        print("Error no data.csv file !")
        return [], []

    except Exception as e:
        print(f"Error reading data.csv : {e}")
        return [], []




def estimate_price(mileage, theta0, theta1):

    return theta0 + (theta1 * mileage)


def user_input(data_mileages=None):

    #get input mileae with validation

    if data_mileages and len(data_mileages) > 0:
        min_km = min(data_mileages)
        max_km = max(data_mileages)
        avg_km = sum(data_mileages) / len(data_mileages)

        print(f"\n Data stored:")
        print(f"   Lowest mileage: {min_km:,.0f} km")
        print(f"   Highest mileage: {max_km:,.0f} km") 
        print(f"   Average mileage: {avg_km:,.0f} km")


    while True:
        try:
            mileage = float(input("Enter the car's mileage (km): "))
            if mileage < 0:
                print("Error mileage cant be negative !")
                continue
            return mileage
        except ValueError:
            print("Error enter a valid number !")




def show_data_stat(mileages, prices):

    if not mileages or not prices:
        return

    print(f"\n Dataset Stat:")
    print(f"   Cars in dataset: {len(mileages)}")
    print(f"   Mileage range: {min(mileages):,.0f} - {max(mileages):,.0f} km")
    print(f"   Price range: ${min(prices):,.0f} - ${max(prices):,.0f}")
    print(f"   Average price: ${sum(prices)/len(prices):,.0f}")




def main():

    #try to load dataset
    data_mileages, data_prices = load_dataset()


    if not data_mileages:
        print("Error getting data ! please check data.csv")
        return
    
    #sho data stat
    show_data_stat(data_mileages,data_prices)

    # try to load trained data
    theta0, theta1, model = load_trained_model()


    if not model :
        print("Will be $0 for any input.")

    # get mileage input from user
    mileage = user_input(data_mileages)

    #calculate price
    estimated_price = estimate_price(mileage, theta0, theta1)

    print(f"Mileage input: {mileage:,.0f} km")
    print(f"Estimated price: ${estimated_price:,.2f}")

    if model :
        print(f"Base price (0 km): ${theta0:,.2f}")

        if theta1 < 0:
            print(f"Price decreases by: ${-theta1:.6f} per km")
        else:
            print(f"Price increases by: ${theta1:.6f} per km")


        #if outside training range
        if data_mileages:
            min_km = min(data_mileages)
            max_km = max(data_mileages)
            if mileage < min_km or mileage > max_km:
                print(f"Accuracy may be reduced !")
    
    else:
        print(f"check program2 !")

    
if __name__ == "__main__":
    main()
