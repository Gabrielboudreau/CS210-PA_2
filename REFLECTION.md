# Reflection Questions

## 1. For every function you wrote (TODO 1 to 19), explain in your own words what it does and give its time complexity in terms of n, the number of scenes. One short paragraph each.

1.1: The constructor creates a scene and allocates the memory needed to
store all possible choices. It initializes the choice pointers to nullptr so all positions
start with a safe, defined value. This prevents them from containing uninitialized addresses. Its time
complexity is (O(1)) because the maximum number of choices per scene is constant and does not
increase in line with the total number of scenes.

1.2: The destructor's job is to free memory. All memory associated with a scene is freed to
prevent memory leaks. It does this by deleting each child scene, then deleting the array that
stores the child pointers, and finally deleting the scene’s data. Each child destructor
recursively handles its own descendants. Its time complexity is (O(n)) because all scenes
in the subtree they're deleted as the destructor walks through them.

1.3: This function checks if a scene is the end of the story by seeing
if it has any child choices. If it does not, the scene is
considered an ending. A scene with one or more choices means the story can
continue. Its time complexity is (O(1)) because it only checks the number of choices
stored in the current scene rather than in all scenes.

1.4: This function takes an index and retrieves the child scene from the choices array.
Before returning the requested scene, it checks if the index is within the valid range and
returns nullptr if the index is not valid. Its time complexity is (O(1)) because accessing an
An individual array element does not require traversal of other scenes.

1.5: This function assigns a child scene to a position in the current scene’s
array of choices. It first checks if the provided index is valid, so the position is in
the current array. The time complexity is (O(1)) because
It does a range check and assigns a pointer to one array position.

1.6: This function recursively counts the total number of scenes in the story from the current starting point.
It returns zero if the scene is nullptr. Otherwise, it counts the current scene
and adds the number of scenes found in each of its child subtrees. Its time complexity is
(O(n)) as each scene is visited just once to work out the final total.

1.7: This function recursively counts the number of ending scenes in the story from the current scene. 
It returns zero if the scene is empty and returns one if the current scene is an ending. It then goes 
on to count all the endings in each of the child subtrees. The overall time complexity is (O(n)) as 
every scene will have to be examined to determine whether or not it is an ending.

1.8: This function also recursively works out each possible height of the story. It then works out the number of choices a player would have before getting to an ending. It returns -1 if the story is empty, 1 if it's an ending and zero otherwise. It works out the height of each child, takes the max of the heights along a path and adds one to that. Time complexity is O(n) as each scene will have to be visited in turn giving us n cases. t1.9: This function frees up each scene in the story and leaves the tree completely empty at the end. It does this by deleting the root scene which in turn sets the root pointer to null.
to nullptr. This allows the scene destructor to recursively delete all descendants and
thereby memfree the memory. Its time complexity is O(n) as every scene in the tree has to be processed. 1.10: This function's job is to load a story from a particular text file and prepare it for use.
it so that it can be used in the final adventure. It opens the file prior to reading the starting health. It calls...
readScene() which will build the story tree. It then assigns the finished tree to the previous adventure.
Its time complexity is (O(n)). Each scene must be read and later must be constructed when loading the
story.

1.11: This function reads information in an individual scene from the story file. It
can use that info so it can construct a scene object. It reads the scene’s text,
health change, loot, item, and number of choices before recursively reading and connecting its
child scenes according to their labels. Its time complexity is (O(n)) because every scene in
the story is processed once during the construction of the tree.

1.12: This function prompts the player to select one of the available choices and ensures that
the selection falls within the permitted range. If the player enters an invalid value,
function displays an error message and continues requesting input until a valid selection is
Provided, the function displays an error message and carries on asking for input until a valid selection has been made (i.e. a valid input attempt has been made). The only way of determining whether a selection is valid is to simply compare it.
it against the number of available choices.

1.13: This function allows the player to progress through the story by displaying each scene
and allowing them to select which direction they want to take. As the player moves through the
tree, their health, loot, and inventory are updated according to the information stored in each
scene, with the game ending when they reach an ending or their health reaches zero. Its time
complexity is (O(n)) in the worst case because a player can visit each scene at most once
during a single playthrough.

1.14: This function displays important statistics about the loaded story, including the total
number of scenes, the number of endings, and the longest possible playthrough. It uses the
recursive helper functions within StoryTree to calculate these values rather than manually
traversing the tree within the function itself. Its time complexity is (O(n)) because each
statistic requires examining the scenes in the tree, and performing these traversals separately
does not alter the overall linear complexity.

1.15: This function provides the player with information about every possible ending in the
story without requiring them to play through each path. It uses the recursive printEndings()
helper to explore the different branches of the tree and display the paths that lead to each
ending. Its time complexity is (O(n^2)) in the worst case when accounting for the additional
The work required to build and copy the path strings (O(n)) although the tree traversal itself is (O(n)).

1.16: This function recursively explores the story tree and prints the path associated with
each possible ending. As it moves through the different choices, it extends the current path
to reflect the decisions made and prints the corresponding scene information once an ending is
reached. Its time complexity is (O(n^2)) in the worst case when accounting for the repeated
copying and extending of path strings whilst actually visiting the scenes themselves is (O(n)) time.

1.17: This function calculates a greedy path through the story by selecting the available choice
that provides the greatest immediate amount of loot. When multiple choices provide the same
amount of loot, it selects the choice with the lowest index, continuing through the tree until
an ending or other stopping condition is reached. Its time complexity is (O(n)) because it
follows a single path through the tree and examines only the available choices at each visited
scene.

1.18: This function recursively examines the possible paths through the story in order to
determine which ending provides the greatest amount of loot while allowing the player to
survive. It keeps track of the player’s health and the current path as it explores each branch,
updating the best path whenever it discovers a more beneficial surviving outcome. Its
time complexity is (O(n^2)) in the worst case when accounting for the additional work involved
in copying and extending path strings, although examining every scene in the tree requires
(O(n)) time.

1.19: This function determines and displays the optimal path through the story by using
bestLoot() to examine the possible outcomes starting from the root scene. Unlike the greedy
approach, it considers the complete possible paths rather than selecting choices based only on
their immediate loot, thereby identifying the ending that provides the greatest total loot while
keeping the player alive. Its time complexity is (O(n^2)) in the worst case when accounting for
path-string operations performed during the recursive search, while the underlying traversal of
the tree is (O(n)).

## 2. Why does each scene allocate exactly numChoices slots instead of declaring Scene<T>* choices[3]? What does an ending hold in choices_, and why?

Each scene allocates exactly numChoices slots because the number of choices can vary between scenes,
and allocating memory dynamically lets each scene store only the number of child pointers it
actually needs. Declaring Scene* choices would limit every scene to three choices, regardless
of how many choices it has, and would also allocate unnecessary space for scenes with fewer
choices. An ending contains an empty choices array (choices_ with zero slots) because it has no
additional scenes branching from it. This lets the program identify it as an ending when
numChoices_ is zero.

## 3. Walk through your Scene destructor. What gets deleted, and in what order? What would go wrong if choices_ were deleted first?

The Scene destructor is responsible for freeing all the memory allocated when a scene and its
choices were created. It first deletes each child scene in choices_, which recursively frees the
descendants of those scenes. Then it deletes the choices_ array itself, and finally deletes the scene’s
data. If choices_ were deleted first, the program would lose access to the child pointers before it
could delete the child scenes, which would cause memory leaks because the memory used for those scenes
would not be properly released.

## 4. Show the path greedy took and the path optimal took on your story, with the loot for each. Explain why greedy lost. If greedy did not lose, change your story until it does, and explain what you changed.

When running my story, the greedy algorithm followed the path 2-2-1, accumulated 0 loot,
died before reaching the ending. In contrast, the optimal algorithm took the path
2-2-1-2-1, collected 15 loot, and reached the ending. Greedy lost because it chose
the options that gave the most loot right away without thinking about the consequences of those
decisions later in the story. The optimal algorithm looked at all possible paths and chose the most
A beneficial outcome that still let the player survive.

## 5. bestLoot tries every path, yet it runs in O(n) time. Explain why it is not exponential, using what you know about paths and leaves in a tree.

Although bestLoot() looks at every possible path in the story, it does not repeat
the work of exploring the same scene more than once. In a tree, each path to an ending follows
branches that together include all the tree’s scenes, and the number of ending paths (leaves) cannot
be more than the total number of scenes. So, the function runs in (O(n)) time because it visits
each scene only once, instead of doing an exponential amount of work.

## 6. Which List methods does your game use, and why? What does one search cost as the bag grows?

My game uses several methods from the List class: search(), addFront(), size(), and print(),
to manage the player’s inventory. The search() method checks if an item is already in
the inventory, addFront() adds a new item, size() shows how many items are currently
stored, and print() displays the inventory to the player. Since the inventory uses a
linked list, searching once takes (O(n)) time in the worst case, where (n) is the number of
items in the bag as the list may need to be traversed until the required item is found or the
end is reached.