import matplotlib.pyplot as plt
import numpy as np

# Read the data from the CSV file
data = np.genfromtxt('plot.csv', delimiter=',')

# Extract X, Y, and noise values
x = data[:, 0]
y = data[:, 1]
z = data[:, 2]

# Create a grid for plotting
X, Y = np.meshgrid(np.unique(x), np.unique(y))
Z = z.reshape(len(np.unique(y)), len(np.unique(x)))

# Plot the Perlin noise
plt.figure(figsize=(8, 6))
plt.contourf(X, Y, Z, levels=50, cmap='viridis')  # Increase the number of levels
plt.colorbar(label='Perlin Noise Value')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Perlin Noise')
plt.show()
