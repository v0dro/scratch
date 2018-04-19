package session01.b_bouncer

import neko._

class Player(p: ProcessConfig) extends ActiveProtocol(p, "pingpong")
{
  listenTo(classOf[Greeting])

  def run() {
    for (p <- neighbors) {
      println(s"send to $p")
      SEND(Greeting(me, p, "PING"))
    }

    for (i <- 1 to neighbors.size) {
      Receive {
        case Greeting(from, _, msg) => println(s"${from.name} replied '$msg' to me (${me.name})")
        case m => println (s"other Message: $m")
      }
    }
  }
}

case class Greeting(from: PID,
                    to: PID,
                    msg: String
                   ) extends UnicastMessage

class Bouncer(p: ProcessConfig) extends ReactiveProtocol(p, "bouncer")
{

  listenTo(classOf[Greeting])

  override def onReceive: PartialFunction[Event, Unit] = {
    case Greeting(from, _, msg) =>
      println(s"I (${me.name}) received '${msg}' from ${from.name} and sending back")
      SEND(Greeting(me, from, s"PONG: $msg"))
    case m => println(s"I (${me.name}) received an unknown message: $m")
  }

  override def onSend: PartialFunction[Event, Unit] = PartialFunction.empty
}


object BounceMain extends Main(topology.Clique(2))(
  ProcessInitializer { p =>
    if (p.pid == PID(0))
      new Player(p)
    else
      new Bouncer(p)
  }
)
