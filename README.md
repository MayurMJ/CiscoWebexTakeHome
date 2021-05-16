# CiscoWebexTakeHome


This project contains solutions to 3 questions.

1.] RTPHandler:
	The first problem contains a solution to parsing a RTP header and mapping
	the count of CSRC fields. I have used a struct rtp to store the values of 
	all the fields in the RTP header packet, along with the extension. I am also
	making use of lambda function for the callback.
	If given more time I would have to understand the use RTP in more detail and
	also would have parsed the actual meaningful values from payload type.

2.] GetMaximumParticipants:
	The second problem is to get the maximum number of participants at any
	time in a meeting. I have made use of a priority queue for this problem:
	I create a min heap to keep track of the lowest leaving time of all ppl
        currently present in some meetings. If the new start time of joining for a
	person is greater than the minimum end from the heap, then we don't need to add
        the person's end time to the heap because someone will leave his meeting and new person
        can take his spot. Whereas if the new start time is lesser than top of the heap we have
        one more person attending a meeting at the same time. The priority_queue at any point of time gives the number of ppl concurrently
        attending a meeting.
	Time Complexity: O(nlog(n)) => sorting OR parsing all n ppl(n) and adding them to queue(log(n))
	Space Complexity: O(n) => maximum size of the priority queue, extra space for the Struct array

3.] SearchInRotatedSortedArray:
	The third assignment is to search for an element in a rotated sorted array. This can be simply done using linear
	search, but I have made use of binary search to expolit sorted the property of the array.
	According to the problem we might find a mid where either first half of array is sorted or the second half is sorted
	If the First half of array is sorted, if the target lies between first and mid search in first half else
        search in second.
	If the Second half of array is sorted, if the target lies between mid and Last, search in second half else
        search in first.
	
	Time Complexity: O(log(n)) required to search the array.
	Space complexity: O(1) No extra space is being used. 

