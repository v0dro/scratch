require 'time'
require 'benchmark'

Benchmark.bm do |x|
  x.report("Creating Time") do
    10000.times { Time.new(2012,1,4,2,2,2) }
  end
  t = Time.new(2012,2,29,4,43,24)

  x.report("Ask Time for month") do
    10000.times { t.month }
  end

  x.report("Ask Time for minutes") do
    10000.times { t.min  }
  end

  dt = DateTime.new(2012,2,29,4,43,24)
  
  x.report("Ask DateTime for month") do
    10000.times { dt.month  }    
  end

  x.report("Ask DateTime for minutes") do
    10000.times { dt.min  }
  end

  x.report("Creating DateTime") do
    10000.times { DateTime.new(2012,1,4,2,2,2)  }
  end
end
