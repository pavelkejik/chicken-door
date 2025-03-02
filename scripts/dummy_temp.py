import numpy as np
import json

# Number of samples
num_samples = 48

# Generate a sine wave
x = np.linspace(0, 2 * np.pi, num_samples)
sin_wave = np.sin(x)

# Scale the sine wave to the 0-14 range
scaled_wave = (sin_wave + 1) / 2 * 14  # Adjusts the wave to go from 0 to 14

# Round to integer values
int_values = np.round(scaled_wave).astype(int)

# Convert to list and then to JSON
temperature_data = int_values.tolist()
json_output = json.dumps({"Temperatures": temperature_data})

print(json_output)
