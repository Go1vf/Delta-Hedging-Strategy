# Delta Hedging Strategy

This repository contains a simulation of the Delta Hedging strategy for options trading, using the Black-Scholes-Merton model for option pricing.

## Table of Contents

- [Overview](#overview)
- [Components](#components)
  - [Option Pricing](#option-pricing)
  - [Delta Hedging](#delta-hedging)
  - [Simulation](#simulation)
  - [Main Execution](#main-execution)
- [Usage](#usage)
- [Contribution](#contribution)
- [License](#license)

## Overview

The project simulates the Delta Hedging strategy for options trading. The main goal is to use the Black-Scholes-Merton (BSM) model to price options and implement the Delta Hedging strategy to offset the risk of price movement in the underlying asset.

## Components

### Option Pricing

- Uses the BSM model to price both call and put options.
- Supports calculations for a group of stock prices or a single stock price.
- Contains methods to find implied volatility.
- Check out the implementation [here](https://github.com/Go1vf/Delta-Hedging-Strategy/blob/main/OptionPrice.cpp).

### Delta Hedging

- Implements the Delta Hedging strategy which involves adjusting the position in an option with respect to the underlying asset.
- Calculates portfolio values and hedging errors.
- Dive into the code [here](https://github.com/Go1vf/Delta-Hedging-Strategy/blob/main/DeltaHedging.cpp).

### Simulation

- Simulates stock prices, option prices, and interest rates.
- Provides functionality to generate output based on the simulations.
- Contains methods to convert date strings to `std::tm` and find data in a specific date range.
- See the simulation code [here](https://github.com/Go1vf/Delta-Hedging-Strategy/blob/main/Simulation.cpp).

### Main Execution

- Integrates all components and runs the simulation.
- Reads data from CSV files and generates output based on the Delta Hedging strategy.
- View the main execution [here](https://github.com/Go1vf/Delta-Hedging-Strategy/blob/main/main.cpp).

## Usage

1. **Clone the Repository**: 
   ```bash
   git clone https://github.com/Go1vf/Delta-Hedging-Strategy.git
