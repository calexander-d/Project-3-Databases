#pragma once

#include "WineReview.h"
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

//bool compare(const WineReview& a, const WineReview& b, const string& field, bool ascending)
//{
//	if (field == "price") 
//	{
//		return ascending ? (a.price < b.price) : (a.price > b.price);
//	}
//	else if(field == "points")
//	{
//		return ascending ? (a.points < b.points) : (a.points > b.points);
//	}
//	else if (field == "region_1")
//	{
//		return ascending ? (a.region_1 < b.region_1) : (a.region_1 > b.region_1);
//	}
//}

int partition(vector<WineReview>& arr, int low, int high, function<bool(const WineReview&, const WineReview&)> compare)
{
	WineReview pivot = arr[high];
	int i = low - 1;

	for (int j = low; j < high; j++)
	{
		if(compare(arr[j], pivot))
		{
			++i;
			swap(arr[i], arr[j]);
		}
	}

	swap(arr[i + 1], arr[high]);
	return i + 1;
}

void medianOfThree(vector<WineReview>& arr, int low, int high, function<bool(const WineReview&, const WineReview&)> compare)
{
	int mid = low + (high - low) / 2;

	if (compare(arr[high], arr[low])) swap(arr[low], arr[high]);
	if (compare(arr[mid], arr[low])) swap(arr[mid], arr[low]);
	if (compare(arr[high], arr[mid])) swap(arr[high], arr[mid]);

	swap(arr[mid], arr[high]);
}

void quicksort(vector<WineReview>& arr, int low, int high, function<bool(const WineReview&, const WineReview&)> compare)
{
	while(low < high)
	{
		medianOfThree(arr, low, high, compare);
		int p = partition(arr, low, high, compare);

		if(p - low < high - p)
		{
			quicksort(arr, low, p - 1, compare);
			low = p + 1;
		}
		else
		{
			quicksort(arr, p + 1, high, compare);
			high = p - 1;
		}
	}
}

void quicksort(vector<WineReview>& arr, const string& field, bool ascending)
{
	function<bool(const WineReview&, const WineReview&)> compare;

	if (field == "price") {
		compare = ascending
			? [](const WineReview& a, const WineReview& b) { return a.price < b.price; }
			: [](const WineReview& a, const WineReview& b) { return a.price > b.price; };
	}
	else if (field == "points") {
		compare = ascending
			? [](const WineReview& a, const WineReview& b) { return a.points < b.points; }
			: [](const WineReview& a, const WineReview& b) { return a.points > b.points; };
	}
	else if (field == "region_1") {
		compare = ascending
			? [](const WineReview& a, const WineReview& b) { return a.region_1 < b.region_1; }
			: [](const WineReview& a, const WineReview& b) { return a.region_1 > b.region_1; };
	}

	if(arr.empty() == false)
	{
		quicksort(arr, 0, arr.size() - 1, compare);
	}
}