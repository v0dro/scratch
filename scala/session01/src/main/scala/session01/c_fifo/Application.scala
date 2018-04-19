package session01.c_fifo

import neko._

class Application (p: ProcessConfig, numIter: Int)
  extends ActiveProtocol(p, "App")
{
  var receivedFrom = Map .empty[PID, List[Long]] .withDefaultValue(Nil)

  listenTo(classOf[Application.GenericMessage])

  override def run ()
  {
    for (i <- 1 to numIter) {
      println(s"Application send... ${me.name} sequence number is $i")
      SEND(Application.GenericMessage(me, neighbors, i))
    }

    for (i <- 1 to numIter) {
      for (j <- neighbors) {
        Receive {
          case m @ Application.GenericMessage(from,_,sn,_) =>
            receivedFrom = receivedFrom.updated(from, sn :: receivedFrom(from))
            if (me == PID(0)) {
              println(s"[${me.name}] message received: $m")
            }

          case _ =>
        }
      }
    }

    println(s"[${me.name}] FINISHED")
    neighbors.foreach { pid =>
      val arrivalSequence = receivedFrom(pid).reverse.mkString(" ")
      println(s"[${me.name}] from ${pid.name} -> $arrivalSequence")
    }
  }
}


object Application
{
  case class GenericMessage(from: PID, to: Set[PID], sn: Long,
                            id: MessageID = MessageID.auto())
    extends MulticastMessage
}


object ApplicationTest
  extends Main(topology.Clique(3))(
    ProcessInitializer { p=>
      val app = new Application(p, 20)
      val fifo = new FIFOChannel(p)
      app --> fifo
    }
  )
