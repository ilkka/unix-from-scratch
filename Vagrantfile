# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant::Config.run do |config|
  config.vm.box = "lucid64"

  config.vm.customize do |vm|
    vm.name = "Unix from Scratch"
  end

  config.vm.provision :chef_solo do |chef|
    chef.cookbooks_path = "cookbooks"
    chef.add_recipe "tools"
    #chef.add_role "web"

    # You may also specify custom JSON attributes:
    #chef.json = { :mysql_password => "foo" }
  end
end
