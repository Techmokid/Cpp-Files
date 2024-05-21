import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

# Load the Perlin noise values from the CSV file
data = np.loadtxt('plot.csv', delimiter=',')

# Create the grid for X and Y
x = []
y = []
z = []
for i in data:
    x.append(i[0])
    y.append(i[1])
    z.append(i[2])

# Set up the plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Plot the points with color based on the Z values
scatter = ax.scatter(x, y, z, c=z, cmap='viridis')  # Use 'viridis' colormap, you can change it to 'jet', 'coolwarm', etc.

# Labels and title
ax.set_xlabel('X Coordinate')
ax.set_ylabel('Y Coordinate')
ax.set_zlabel('Z Coordinate')
ax.set_title('3D Point Plot Colored by Height')

# Color bar
cbar = fig.colorbar(scatter, ax=ax, pad=0.1)
cbar.set_label('Height')

# Show the plot
plt.show()
