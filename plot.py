import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('waiting_probability_results.csv')

# Create the plot with a specific size and DPI
plt.figure(figsize=(10, 7), dpi=100)

# Define colors and markers
colors = ['blue', 'red', 'brown', 'black']
markers = ['o', 'o', 'o', 'o']

# Plot for each combination of w and N
for i, (w, N) in enumerate([(5, 5), (5, 10), (10, 5), (10, 10)]):
    data = df[(df['MeanPatienceTime'] == w) & (df['Operators'] == N)]
    plt.plot(data['OfferedLoad'], data['WaitingProbability'], 
             color=colors[i], marker=markers[i], markersize=6, 
             linewidth=1.5, label=f'w = {w}, N = {N}')

plt.xlabel('Offered Load (Erlangs)', fontsize=12)
plt.ylabel('Probability of Customer Waiting', fontsize=12)
plt.title('Waiting Probability vs. Offered Load', fontsize=14)

# Customize the legend
plt.legend(fontsize=10, loc='upper left')

# Customize the grid
plt.grid(True, linestyle='--', alpha=0.7)

# Set axis limits
plt.ylim(0, 1)
plt.xlim(1, 20)

# Customize ticks
plt.xticks(range(1, 21), fontsize=10)
plt.yticks([0, 0.2, 0.4, 0.6, 0.8, 1.0], fontsize=10)

# Add a light grey background to the plot area
plt.gca().set_facecolor('#f0f0f0')

# Adjust layout and save
plt.tight_layout()
plt.savefig('waiting_probability_plot.png', dpi=300, bbox_inches='tight')
plt.show()