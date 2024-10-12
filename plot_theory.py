import numpy as np
import matplotlib.pyplot as plt
from scipy.special import factorial

def erlang_b(a, n):
    """
    Calculate the Erlang B formula
    a: offered load in Erlangs
    n: number of channels
    """
    numerator = (a**n) / factorial(n)
    denominator = sum((a**i) / factorial(i) for i in range(n+1))
    return numerator / denominator

# Define the range of channels and offered loads
channels = range(1, 21)
offered_loads = range(1, 21)

# Create a new figure with a larger size
plt.figure(figsize=(12, 8))

# Calculate and plot for each offered load
for a in offered_loads:
    blocking_probs = [erlang_b(a, n) for n in channels]
    plt.plot(channels, blocking_probs, marker='o', label=f'a = {a}')

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

# Use a color scheme similar to the original plot
plt.gca().set_prop_cycle(color=plt.cm.rainbow(np.linspace(0, 1, 20)))

plt.savefig('erlang_b_theoretical_plot.png', bbox_inches='tight', dpi=300)
plt.show()