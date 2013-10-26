## Backup

Major Goals:
	- Deduplication
		- As few blocks should be stored as possible.
	- Redundancy
		- Should tolerate data failures, whether due to hardware or hash collision.
	- Tiered storage:
		- Client only maintains hash table, datablocks can be mitigated entirely to remote host.
		- In the event of a hash collision, redundancy should cover at least one block failure.
	- Sliding window:
		- Deduplication should not be bound to fixed intervals. (Though fixed window size is acceptable)

Originally the weak hash for deduplication was going to be based off of a rolling adler32. The current
implementation requires too much shuffling (maxing around 60 MB/s of hash throughput), so most of it will likely be
rewritten in the near future with a fletcher hash and no character queues.
