#ifndef ARTIFICIAL_STOCKMARKET_H
#define ARTIFICIAL_STOCKMARKET_H


// Include statements
#include "RandomUtils.h"
#include <vector>
#include <cmath>


namespace artificialMarket {
	// Configuration struct to hold modifiable parameters
    struct MarketConfig {
        float MIN_PRICE = 0;
        float MAX_PRICE = 100;
        size_t MAX_MARKET_HISTORY = 1000;
        float VOLATILITY = 0.01;
    };

    namespace _internal {
        inline MarketConfig config;
        inline float oldPrice = (config.MAX_PRICE + config.MIN_PRICE)/2;
        inline std::vector<float> marketData;
    }
	
    void SetMarketConfig(const MarketConfig& newConfig) {
        _internal::config = newConfig;
        _internal::oldPrice = (_internal::config.MAX_PRICE + _internal::config.MIN_PRICE) / 2; // Reset the oldPrice based on new config
    }
	
	
	// Function Definitions
	void UpdateMarket() {
		float changePercent = 2*_internal::config.VOLATILITY*(GetRandomFloat() - 0.5);
		float changeAmount = _internal::oldPrice * changePercent/100;
		float newPrice = _internal::oldPrice + changeAmount;

		if (newPrice < _internal::config.MIN_PRICE)
			newPrice += std::abs(changeAmount) * 2;
		else if (newPrice > _internal::config.MAX_PRICE)
			newPrice -= std::abs(changeAmount) * 2;
		
		if (_internal::marketData.size() == 0)
			_internal::marketData.push_back(_internal::oldPrice);
		
		_internal::marketData.push_back(newPrice);
		
		if (_internal::marketData.size() > _internal::config.MAX_MARKET_HISTORY)
			_internal::marketData.erase(_internal::marketData.begin());
		
		_internal::oldPrice = newPrice;
	}

	float GetMarketPrice() { return _internal::oldPrice; }
	std::vector<float> GetFakeMarketHistory() { return _internal::marketData; }

	float GetAndUpdateMarketPrice() {
		UpdateMarket();
		return GetMarketPrice();
	}
}
#endif