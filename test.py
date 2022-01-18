from subprocess import Popen, PIPE
import logging

logging.basicConfig(format="%(levelname)s: %(message)s",level=logging.DEBUG)
log = logging.getLogger(__name__)

def make_target(target):
    log.debug("make {}".format(target))
    process = Popen(['make', target], stdout=PIPE, stderr=PIPE)
    process.wait()
    log.info(process.stdout.read().decode("utf-8"))
    
make_target("all")

log.debug("run server")
process_server = Popen(['./app','server'], stdout=PIPE, stderr=PIPE)

log.debug("run client")
process_client = Popen(['./app','client'], stdout=PIPE, stderr=PIPE)

#log.info("process server: %s", process_server.stdout.read().decode("utf-8"))
log.info("%s", process_server.communicate())
#process_server.kill()
