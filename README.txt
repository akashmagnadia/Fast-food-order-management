Commnd - Description  

? - List the commands used by this program and a brief description of how to use each one.  

a<#burgers><#salads><name> - Add the orderto the orderlist using the given orderand name specifying the group is waiting in the restaurant.  The order’s information is added to the end of the list.  If the name already exists in the orderlist, give an error message and do not add the information.  

c <#burgers><#salads><name> - Add the orderto the orderlist using the given orderand name specifying the orderas a call ahead order.  The order’s information is added to the end of the list.  If the name already exists in the orderlist, give an error message and do not add the information.  

w <name> - Mark the call ahead orderusing the given name as waiting in the restaurant.  If the name does not exist is the orderlist or is not a call ahead group, give an error message.  

r <#burgers><#salads> - Retrieve and remove the first orderon the orderlist that is waiting in the restaurant and containsless than or equal to number of prepared burgers and salads.  Note that “first” is the order that has been in the orderlist the longest.  

l <name> - List total number of orders that are in the orderlist in front of the orderspecified by the given name. If the name does not exist, give an error message.  

t<name> - Give an estimated waiting time based on the order list knowing preparing burgers will take 10 minutes and preparing salads takes 5.If the name does not exist, give an error message.  

d - Display the total number of orders in the orderlist.  Also display the names, order detailsand in-restaurant status of all ordersin the orderlist in order from first to last.  

Note that <#burgers>and <#salads>are to be integer values and <name>is a list of characters.The < and > symbols are NOT part of the input but being used to describe the input.
