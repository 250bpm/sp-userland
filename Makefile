all: pushpull_server pushpull_client pubsub_server pubsub_client reqrep_server reqrep_client


clean:
	$(RM) pushpull_server pushpull_client pubsub_server pubsub_client reqrep_server reqrep_client
