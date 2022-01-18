from subprocess import Popen, PIPE
import logging

logging.basicConfig(format="%(levelname)s: %(message)s",level=logging.DEBUG)
log = logging.getLogger(__name__)

def make_target(target):
    log.debug("make {}".format(target))
    process_server = Popen(['make', target], stdout=PIPE, stderr=PIPE)
    process_server.wait()
    log.info(process_server.stdout.read().decode("utf-8"))
    
make_target("server")
make_target("client")

#run
log.debug("run server")
process_server = Popen(['./server'], stdout=PIPE, stderr=PIPE)

log.debug("run client")
process_client = Popen(['./client','127.0.0.1'], stdout=PIPE, stderr=PIPE)
stdout, stderr = process_client.communicate()
print("process_client: {}".format(stdout.decode("utf-8")))

process_server.kill()
