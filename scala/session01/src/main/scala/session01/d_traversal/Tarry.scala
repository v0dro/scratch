package session01.d_traversal

import neko._
import neko.util.TraversalClient


class Tarry (p:ProcessConfig)
  extends ReactiveProtocol(p, "Tarry")
{
  import Tarry._
  import TraversalClient._

  private def isRoot = parent == me
  private var parent : Option[PID] = None
  private var children = List.empty[PID]

  private def visitNextChild() {
    children match {
      case next :: tail    => { // rule 1
        SEND(Token(me, next))
      }
      case Nil if ! isRoot => SEND(Token(me, parent.getOrElse(PID(0)))) // rule 2
      case Nil if isRoot   => DELIVER (Done)       // End
    }
  }

  def onSend = {
    case Initiate =>
      parent   = Some(me)
      children = neighbors.toList
      DELIVER (Visit)
      visitNextChild()
  }

  listenTo(classOf[Token])
  def onReceive = {
    case Token(from, _) if parent.isEmpty =>
      parent   = Some(from)
      children = (neighbors-from).toList
      DELIVER (Visit)
      visitNextChild()
    case Token(_, _) => visitNextChild()
  }
}


object Tarry
{
  case class Token(from: PID, to: PID)
    extends UnicastMessage
}


object TarryMain
  extends Main(topology.Grid(4, 4))(
    ProcessInitializer { p =>
      val app   = new AppTraversal(p)
      val tarry = new Tarry(p)
      app --> tarry
    }
  )
