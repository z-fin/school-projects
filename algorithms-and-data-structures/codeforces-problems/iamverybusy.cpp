/*
/
/   You are actually very busy man. You have a big schedule of activities. Your aim is to do as much as activities as possible.
/   In the given figure, if you go to date with crush, you cannot participate in the coding contest and you can’t watch the movie.
/   Also if you play DotA, you can’t study for the exam. If you study for the exam you can’t sleep peacefully.
/   The maximum number of activities that you can do for this schedule is 3. 
/
/   Either you can:
/       - watch movie, play DotA and sleep peacefully (or)
/       - date with crush, play DotA and sleep peacefully
/
/   Submission: https://www.spoj.com/TDA242C1/files/src/33462722/
/   
*/
#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
struct Activity
{
    int start;
    int end;
};
 
bool compareByEndTime(const Activity &a, const Activity &b)
{
    return a.end < b.end;
}
 
class BusyMan
{
private:
    int n;
    vector<Activity> activities;
 
public:
    BusyMan(int n) : n(n)
    {
        activities.resize(n);
 
        for (int i = 0; i < n; i++)
        {
            cin >> activities[i].start >> activities[i].end;
        }
    }
 
    void resolve()
    {
        sort(activities.begin(), activities.end(), compareByEndTime);
        vector<Activity> viableActivities(activities);
        int activityCounter = 0;
 
        while (!viableActivities.empty())
        {
            activityCounter++;
            int endOfLastActivity = viableActivities[0].end;
            viableActivities.erase(viableActivities.begin());
 
            viableActivities.erase(
                remove_if(viableActivities.begin(), viableActivities.end(),
                          [endOfLastActivity](const Activity &a)
                          {
                              return a.start < endOfLastActivity;
                          }),
                viableActivities.end());
        }
 
        cout << activityCounter << endl;
    }
};
 
int main()
{
    int t;
    
    cin >> t;
 
    while (t > 0)
    {
        int n;
 
        cin >> n;
 
        BusyMan busyMan(n);
        busyMan.resolve();
 
        t--;
    }
 
    return 0;
} 