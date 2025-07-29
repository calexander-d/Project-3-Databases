using namespace std;

#ifndef WINEREVIEW_H
#define WINEREVIEW_H

struct WineReview {
    string country, description, designation, province, region_1;
    string taster_name, taster_twitter_handle, title, variety, winery;
    int points;
    float price;
    WineReview() {}
};

#endif //WINEREVIEW_H
