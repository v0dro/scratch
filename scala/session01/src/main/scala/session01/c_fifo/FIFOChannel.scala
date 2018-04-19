package session01.c_fifo

import neko._

import scala.collection.mutable

/**
  * Implements multi-party FIFO channels.
  * The protocol is not thread-safe if each process has more than one send thread
  * or more than one receive thread.
  *
  * @param p
  */
class FIFOChannel (p: ProcessConfig)
  extends ReactiveProtocol(p, "FIFO channels")
{
  import FIFOChannel._

  // TODO: add necessary state variables

  def onSend = {
    case m : Message =>
      // TODO: implement the proper sending of a message
      m.destinations foreach {
        dest =>
          SEND( FIFOSequenced(me, dest, m, 0) )
      }
  }

  listenTo(classOf[FIFOSequenced])

  def onReceive = {
    case FIFOSequenced(from, to, m, sn, _) =>
      // TODO: implement the correct reception of a message
      DELIVER(m)

    case _ => /* IGNORE */
  }
}


object FIFOChannel
{
  case class FIFOSequenced(
                            from: PID, to: PID,
                            payload: Message, seqNum: Long,
                            id: MessageID = MessageID.auto()
                          ) extends UnicastMessage

  case class FIFOInfo (from: PID, seqNum: Long, msg: Message)

  val FIFOOrdering =
    Ordering[(Long, Int)]
      .on((info: FIFOInfo) => (-info.seqNum, info.from.value))
}


object FIFOTestMain
  extends Main(topology.Clique(5))(
    ProcessInitializer { p=>
      val app  = new Application(p, numIter = 20)
      val fifo = new FIFOChannel(p)
      app --> fifo
    }
  )
