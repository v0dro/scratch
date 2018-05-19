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
      //init()
      SEND(Start(me, PID(0)))
    }
  }

  listenTo(classOf[Computation])
  override def onReceive = {
    case Computation(_,_,res) =>
      println("The result of computation from all nodes is :" + res)
    case _ => // do nothing
  }
}

