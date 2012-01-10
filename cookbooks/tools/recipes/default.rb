bash "update apt-get db" do
  user 'vagrant'
  code <<-EOS
  sudo apt-get update
  EOS
end

bash "install build-essential" do
  not_if("dpkg --get-selections build-essential|grep -q install")
  user 'vagrant'
  code <<-EOS
  sudo apt-get install -y build-essential
  EOS
end

bash "install nasm" do
  not_if("dpkg --get-selections nasm|grep -q install")
  user 'vagrant'
  code <<-EOS
  sudo apt-get install -y nasm
  EOS
end

bash "install bochs" do
  not_if("dpkg --get-selections bochs|grep -q install")
  user 'vagrant'
  code <<-EOS
  sudo apt-get install -y bochs
  EOS
end

bash "install vim-nox" do
  not_if("dpkg --get-selections vim-nox|grep -q install")
  user 'vagrant'
  code <<-EOS
  sudo apt-get install -y vim-nox
  EOS
end

