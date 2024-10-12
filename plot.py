import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('max_load_results.csv')

# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(df['Channels'], df['MaxOfferedLoad'], marker='o')

plt.xlabel('Number of Channels')
plt.ylabel('Maximum Offered Load (Erlangs)')
plt.title('Maximum Offered Load vs. Number of Channels for 1% Blocking Probability')
plt.grid(True)

plt.tight_layout()
plt.savefig('max_load_vs_channels.png', dpi=300)
plt.show()