// Name : Sameer Deshmukh
// Student ID: 17M38101
// Parallel broadcast and convergecast code in scala.


package parallel_bcast.parallel_bcast

import neko._

class BcastApp(p: ProcessConfig) extends ActiveProtocol(p, "bcast ccast")
{
  import ParallelBcast._

  def run() {
    if (me == PID(0)) {
      SEND(Start(me, PID(0)))
    }
  }
}

