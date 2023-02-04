#pragma once

/*
	Asset handling for another-engine.

	Assets should have a standard API to load from file or from memory.
	All asset types should be managed here.
	
	TODO: Virtual filesystem type structure?
	TODO: Some kind of map to find assets by name?
	TODO: Load from memory buffer
*/

#include <vector>
#include <map>
#include <string>
#include <concepts>


class IAsset {
public:
	bool _is_loaded = false;
	std::string path;
	std::string asset_type;
	IAsset(std::string _asset_type) : asset_type(_asset_type) {};

	virtual void load_from_file(const char* filename) = 0;
	virtual void unload() = 0;

	inline bool is_loaded() { return _is_loaded; };
};


template <typename T>
concept Asset = std::derived_from<T, IAsset>;


class AssetManager {
private:
	std::vector<IAsset*> assets;

public:
	template <Asset T>
	T* LoadAsset(const char* filename) {
		T* asset = new T();
		asset->load_from_file(filename);
		assets.push_back(asset);
		return asset;
	}
};