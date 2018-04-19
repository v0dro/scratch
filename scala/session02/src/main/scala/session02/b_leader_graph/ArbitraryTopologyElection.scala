/**
 *
 * Copyright 2015 Xavier Defago
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Created by IntelliJ IDEA.
 * User: defago
 * Date: 01/07/15
 * Time: 12:46
 *
 */
package session02.b_leader_graph

import neko._


class ArbitraryTopologyElection (p: ProcessConfig)
  extends ReactiveProtocol(p, "election (arbitrary)")
{
  import ArbitraryTopologyElection._
  import neko.util.LeaderElectionClient._

  private def isRoot = parent == me
  private var parent = me
  private var children = neighbors.toList
  // TODO: declare necessary state variables


  private def visitNextChild() {
    children match {
      case next :: tail    => /* TODO */  // Rule 1
      case Nil if ! isRoot => /* TODO */  // Rule 2
      case Nil if isRoot   => /* TODO */  // End
    }
  }


  def onSend = {
    case Candidate => // TODO: implement
  }

  listenTo(classOf[Token])
  listenTo(classOf[AnnounceLeader])

  def onReceive = ???  // TODO: implement
}


object ArbitraryTopologyElection
{
  case class Token(from: PID, to: PID, candidate: PID) extends UnicastMessage
  case class AnnounceLeader(from: PID, to: Set[PID], leader: PID) extends MulticastMessage

  sealed abstract class StateColor
  case object Red   extends StateColor
  case object Black extends StateColor
}


object ArbElectionMain
  extends Main(topology.Ladder(16))(
    ProcessInitializer { p=>
      val app    = new LeaderApp(p)
      val leader = new ArbitraryTopologyElection(p)
      app --> leader
    }
  )

