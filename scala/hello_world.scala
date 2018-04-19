import neko._

class HelloProg(p: ProcessConfig) extends ActiveProtocol(p, nickname="hello")
{
  def run(): Unit = {
    println(s"Hello from ${me.name} ($me)")
  }
}

object HelloMain extends Main(topology.Clique(2)) {
  ProcessInitializer { p => new HelloProg(p) }
}
