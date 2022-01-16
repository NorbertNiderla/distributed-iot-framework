from subprocess import Popen, PIPE
import logging

logging.basicConfig(format="%(levelname)s: %(message)s",level=logging.DEBUG)
log = logging.getLogger(__name__)


#build
log.debug("make server")
process_server = Popen(['make', 'server'], stdout=PIPE, stderr=PIPE)
process_server.wait()
log.info(process_server.stdout.read().decode("utf-8"))

log.debug("make client")
process_client = Popen(['make', 'client'], stdout=PIPE, stderr=PIPE)
process_client.wait()
log.info(process_client.stdout.read().decode("utf-8"))

#run
log.debug("run server")
process_server = Popen(['./server'], stdout=PIPE, stderr=PIPE)

log.debug("run client")
process_client = Popen(['./client','127.0.0.1'], stdout=PIPE, stderr=PIPE)
stdout, stderr = process_client.communicate()
log.info("process_client: %s", stdout.decode("utf-8"))

process_server.kill()
