#include <stdio.h>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <iostream>
#include <queue>


/*
 * Comparator for priority_queue
 * Gives more priority to elements with less value.
 */ 
 
class cmp{
    public:
    bool operator () (int &a, int &b) {
        return a > b;
    }
};


// Struct to note down the start and end times of a meeting

struct MeetingRange {
    int start;
    int end;
    MeetingRange(int start, int end) : start(start), end(end) {}
};


/*
 *  this function returns the maximum number of participants present
 *  in a meeting at any given time.
 *  
 *
*/

int getMaxParticipants(std::vector<int> join, std::vector<int> leave) {
    assert(join.size() == leave.size());
    if( join.size() == 0) return 0;
    std::vector<MeetingRange> times; 
    for(int i = 0; i < join.size(); i++) {
        times.push_back(MeetingRange(join[i], leave[i]));
    }
    
    // sort the array based on their start times
    std::sort(times.begin(), times.end(), [] (MeetingRange &a, MeetingRange &b) { return a.start < b.start; });
    
    // create a min heap to keep track on minimum leaving time of a person
    // If the new start time of joining of a person is greater than the minimum
    // end time we don't need to add the person's end time to the priority_queue
    // The priority_queue at any point of time gives the number of ppl concurrently
    // attending a meeting.
    std::priority_queue<int, std::vector<int>, cmp > pq;
    pq.push(times[0].end);
    for(int i = 1; i < times.size(); i++) {
        if(times[i].start > pq.top()) {
            pq.pop();
        } 
        pq.push(times[i].end);
    }
    
    return pq.size();
}


int main()
{
    assert(getMaxParticipants({1, 2, 9, 5, 5},
                              {4, 5, 12, 9, 12}) == 3); // Input case
                              
    assert(getMaxParticipants({1, 2, 3, 4, 5},          // Case where second person joins at the same time
                              {2, 3, 4, 5, 6}) == 2);   // as first person leaves

    assert(getMaxParticipants({1, 3, 5, 7, 9},			// Disjoint join/leave times
                              {2, 4, 6, 8, 10}) == 1);
                              
    assert(getMaxParticipants({1, 6},					// Disjoint join/leave times with different array size
                              {5, 10}) == 1);

    assert(getMaxParticipants({1, 2, 3, 4, 5},			// All inclusive
                              {10, 9, 8, 7, 6}) == 5);
    
    assert(getMaxParticipants({},						// Empty scenario
                              {}) == 0);
                              
    assert(getMaxParticipants({5, 8, 4, 3, 2, 1},		// Unsorted inclusive
                              {6, 15, 5, 6, 9, 20}) == 5);
                          
    assert(getMaxParticipants({1,2},					// Larger time range values
                              {1000, 2000}) == 2);
                              
    return 0;
}
