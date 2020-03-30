require 'wanikani'

c = Wanikani::Client.new api_key: "bab0f5a9f4049dc27870b01427d33747"
fname = File.new "critical.csv"
fname << "expression,meaning,reading,sentence\n"
# hsh = c.srs_items_by_type("apprentice")
crits =  c.full_critical_items_response
crits['requested_information'].each do |info|
  
end


fname.close
