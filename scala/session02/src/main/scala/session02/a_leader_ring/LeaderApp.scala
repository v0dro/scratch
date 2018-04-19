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
 * Date: 24/06/15
 * Time: 13:06
 *
 */
package session02.a_leader_ring

import neko._


class LeaderApp(p: ProcessConfig)
  extends ActiveProtocol(p, "leader app")
    with neko.util.LeaderElectionClient
{
  import neko.util.LeaderElectionClient._

  def run() {
    candidate()
    Receive {
      case Elected(Some(leader)) if leader == me =>
        println(s"${me.name} :> I AM THE LEADER (${me.name}) !!!!")

      case Elected(Some(leader)) =>
        println(s"${me.name} :> the leader is ${leader.name}")

      case Elected(None) =>
        println(s"${me.name} :> I'm not a leader")
    }
  }
}
