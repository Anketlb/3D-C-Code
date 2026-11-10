So My parser is slow because of how i am using the vectors, i start with an empty vector than as the program runs the vector changes in size up to the maximum of three. 
than it deletes itself and rewrites a new set of three. this is slow
we could instead use Vector3 which is the fixed size of three and will make it faster because we aren't resizing the vector we can just fill up the vector.
that is for reading the file.

when we go to make the triangle... we also could use vector3 and grab the first value so that it knows what to grab...
