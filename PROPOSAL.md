# Final Project Proposal

## Group Members:

Kevin Chen

# Intentions:

Making a terminal-based chat program

# Intended usage:

A server connects client data (files)

# Technical Details:

**1.** The chat program server will accept clients which write to it.

**2.** 
- Semaphores to organize read-write order
- Sockets to connect client-server
- Allocating memory in case there needs to be non-stack storage
- Signal handling for keys
- May use shared mem if processes inside computer must communicate

**3.** Project Files : Server + Client + Helper(s) to confirm handshake and check status / activity



# Intended pacing:
Jan 5: Planning on what system features are needed 
Jan 6: Start helper file + .h libraries
Jan 7: Make server file + client file using helpers
Jan 8: Finish server and client
