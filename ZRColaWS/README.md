# ZRCola Web Service

## Building and Installation

1. Install prerequisites: `sudo dnf install git cmake gcc gcc-c++ make libatomic`

2. Clone this Git repository: `git clone --recursive https://github.com/Amebis/ZRCola.git`

3. Build ZRColaWS: `make -C ZRCola/ZRColaWS`

4. Install ZRColaWS: `sudo make -C ZRCola/ZRColaWS install`

5. Configure ZRColaWS by editing _/usr/local/etc/sysconfig/zrcolaws_

6. Start ZRColaWS: `sudo systemctl start zrcolaws.service`

7. Configure ZRColaWS for auto-start: `sudo systemctl enable zrcolaws.service`

8. Remember to open webservice port in firewall, should the service require external access.

## Usage

Web service API documentation is available at _http://host:port/swagger/ui_.
