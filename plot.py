import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
df = pd.read_csv('simulation_results.csv')

# Create a new figure with a larger size
plt.figure(figsize=(12, 8))

# Define colors for the lines
unique_loads = sorted(df['OfferedLoad'].unique())
colors = plt.cm.rainbow(np.linspace(0, 1, len(unique_loads)))

# Plot for each unique offered load
for i, load in enumerate(unique_loads):
    data = df[df['OfferedLoad'] == load]
    
    # Plot simulated results
    plt.plot(data['Channels'], data['SimulatedBlockingProb'], 
             marker='o', color=colors[i], label=f'a = {load}')

plt.xlabel('Number of Channels')
plt.ylabel('Blocking Probability')
plt.title('Blocking Probability vs. Number of Channels for Various Offered Loads')
plt.legend(title='Offered Load (Erlangs)', ncol=4, loc='lower left', bbox_to_anchor=(0, -0.4))
plt.grid(True)
plt.yscale('log')  # Use log scale for y-axis
plt.ylim(1e-4, 1)  # Set y-axis limits
plt.xlim(1, 20)    # Set x-axis limits

# Adjust layout to prevent cutting off labels
plt.tight_layout()

# Add more ticks on x-axis
plt.xticks(range(1, 21))

plt.savefig('simulated_blocking_probability_vs_channels.png', bbox_inches='tight', dpi=300)
plt.show()