# W8List

What it does
-----------

  Basically my plans for this, is to use the Google gumbo library to parse the Drexel TMS website (Basically the view 
  for the database of available classes) to see if the Class has opned up yet, or if its still closed. 
  
  Really useful for classes which dont have a waitlist, which is stupid
  
  
What needs to get done
---------------------

  So in its current status, i have it parsing into the HTML to find the right nodes in the HTML of the document
  what i need to add to acutally get it into a useful little tool is as follows...
  * Implement some error checking if/when the servers are down,
  * Get a parent process for the children, to manage the Processes that are watching the individual sites
  * A UI would be nice :3
