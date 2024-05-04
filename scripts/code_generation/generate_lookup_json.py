import json
import os

def load_json(file_path):
    #Load and ret data from JSON file
    with open(file_path, 'r') as file: #Open file in read mode
        return json.load(file) #Parses JSON and loades into Python obj 

def merge_data(data_list):
   #Merge data together to a dictionary, merges everything together
    merged_data = {}
    for data in data_list: #A list of dictionaries. Each dictionary contains separate JSON file. Goes thru each
        for key, value in data.items(): #Iterates key value pairs
            if key in merged_data:
                # Assuming the values are lists and can be extended
                merged_data[key].extend(value)
            else:
                merged_data[key] = value
    return merged_data

def merge_additional_data(existing_data, additional_data_list, additional_data_filenames, master_messages):
    #Merge additional data only if the prefix of the file and the prefix and suffix of the master messages match.

    # Create a set of valid (prefix, suffix) tuples from master messages
    valid_prefix_suffix_pairs = {(msg.split('_', 1)[0], msg.split('_', 1)[1]) for msg in master_messages if '_' in msg}

    for data, filename in zip(additional_data_list, additional_data_filenames):
        # Extract the prefix from the filename
        file_prefix = filename.split('/')[-1].split('_')[0]

        # Check against each valid prefix-suffix pair
        for prefix, suffix in valid_prefix_suffix_pairs:
            if file_prefix == prefix and suffix in data:
                # Initialize the prefix key in existing_data if not present, ensures place to store data
                if prefix not in existing_data:
                    existing_data[prefix] = {}

                # Merge data carefully under the specific prefix and suffix
                if suffix not in existing_data[prefix]:
                    existing_data[prefix][suffix] = data[suffix]
                else:
                    for key, value in data[suffix].items():
                        if key in existing_data[prefix][suffix]:
                            if isinstance(value, list) and isinstance(existing_data[prefix][suffix][key], list):
                                existing_data[prefix][suffix][key].extend(value)
                            else:
                                existing_data[prefix][suffix][key] = value
                        else:
                            existing_data[prefix][suffix][key] = value

    return existing_data

def save_json(data, file_path):
   #Save dictionary to JSON in new file
    with open(file_path, 'w') as file:
        json.dump(data, file, indent=4)

def main():
    #Use master data to only extract relevant info
    master_data = load_json('can_bus/quadruna/VC/VC_rx.json') 

    master_messages = master_data['messages']
    # List of JSON file paths to load
    merge_completely_json = [
        'Consolidated-Firmware/can_bus/quadruna/VC/VC_tx.json'
    
    ]

    merge_partially_json = [
        'can_bus/quadruna/BMS/BMS_tx.json',
        'can_bus/quadruna/CRIT/CRIT_tx.json',
        'can_bus/quadruna/FSM/FSM_tx.json', 
        'can_bus/quadruna/INVL/INVL_tx.json', 
        'can_bus/quadruna/RSM/RSM_tx.json', 
    ]

    # Load data from each file, merge completely for some
    initial_data_list = [load_json(file) for file in merge_completely_json if os.path.exists(file)]
    merged_json =  merge_data(initial_data_list)

    additional_data_list = [load_json(file) for file in merge_partially_json if os.path.exists(file)]
    merged_json = merge_additional_data(merged_json, additional_data_list,merge_partially_json, master_messages)
    # Save the merged data to a new file
    save_json(merged_json, 'software/tracksight/backend/app/process/lookup.json')

if __name__ == "__main__":
    main()

