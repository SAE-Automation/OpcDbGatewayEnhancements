# OpcDbGatewayEnhancements

*OpcDbGateway enhancements, configurations and process databases*

## What is OpcDbGateway

OpcDbGateway enables integration of software applications to collect, process and visualize data from different devices and data sources.

It is *application software development environment* consisting of IDE (GUI) for creating and debugging applications and a runtime engine/application without IDE. GUI is a configuring application (Configurator) that enables configure connections to different data sources as databases, OPC server/client applications, DDE servers using different wizards providing high effectivity. It also enables to configure application logic using configurable commands. The base functionality of the product can be *enhanced by custom programmed modules*. This repository contains examples of custom programmed modules configurations and process databases.

## Product information, download, documentation

* [Product web](http://www.saeautom.sk/en/products/opcdbgateway/)
* OpcDbGateway downloads: [first](http://www.saeautom.sk/download/products/opcgway/OpcDbGateway_Setup.zip),
  [second](http://www.sae-automation.dtadsl.sk/products/opcgway/OpcDbGateway_Setup.zip)
* Documentation [pdf](http://www.saeautom.sk/download/help/opcdbgateway_en.pdf), [web](http://home.gts.sk/saeautomation/help/opcdbgateway/index.html)

## SAEAUT SCADA

SAEAUT SCADA software package has the same functionality as OpcDbGateway but it contains also universal web UI.

* [Product web](http://www.saeautom.sk/sk/products/scada)
* SAEAUT SCADA downloads: [first](http://www.saeautom.sk/download/products/opcgway/SAEAUT_SCADA_Setup.zip),
  [second](http://home.gts.sk/saeautomation/products/opcdbgw/SAEAUT_SCADA_Setup.zip)
* Documentation [pdf](http://www.saeautom.sk/download/help/SaeautScadaHelp.pdf), [web](http://home.gts.sk/saeautomation/help/saeautscada2/index.html)

## Enhancement DLL demo

An example of enhancement DLL can be found in installation packages of above-mentioned products. Other enhancements will be published in this GitHub repository. Here will be published also links to OpcDbGateway enhancement DLLs, configuration databases and process databases from external contributors.

## How to run applications from these repositories

Standard installation places different files and directories to your  directory structure. 	Because of this, when you use demo configurations and database from these GitHub repositories it is necessary to adapt following settings:

* **Placement of data produced by application** e.g. log files: in my case, they are placed in directory *C:\Users\palacka.SAEAUTOM2\Documents\SAEAUT SCADA\Data* as can be seen in Figure 1. After installation SAEAUT SCADA, you can see default placement of your directories by using installed demo-configuration ExampleConfiguration.ODG. You can copy/paste this directory to a configuration that you have downloaded from this Github repository

*Figure 1. Path to log files and other data directories* <img src="LogLocation.png" width="400" height="100" />) 




* **Connect string to default process database** - you can also use connect string from installed ExampleConfiguration.ODG like in Figure 2.

*Figure 2. Connection string to log files and other data directories* <img src="ConnString.png" width="400" height="100" />


Anyway, before using any configuration from this Github repository, it is useful to use the configuration checking functionality of the OpcDbGateway configurator and solve all errors and warnings.

## Project repositories

### [OpcDbGatewayToMQTT](https://github.com/SAE-Automation/OpcDbGatewayEnhancements/tree/master/OpcDbGatewayToMQTT)

Easy configuration for publishing of messages from OpcDbGateway runtime to MQTT broker.

### [Simulation of simple dynamic system](https://github.com/jakubpalacka/MyFirstStepsWithOpcDbGateway)

For those who are starting to develop their own applications using OpcDbGateway, it may be interesting to have a detailed description of each step in creating an application to simulate a simple dynamic system - processed by high school student. It demonstrates:

* Basic concepts of implementing application logic using configurator,
* connecting of DDE servers,
* working with process database,
* visualization of OpcDbGateway application using MS Excel.

### [Using standard streams of a child process to implement plugins](https://github.com/SAE-Automation/OpcDbGatewayEnhancements/tree/master/ParamChildStdOutReader)

The project demonstrates:

* Creation of plugins for OpcDbGateway using standard streams,
* parameterizing of continuously running plugin from configuration,
* publish/subscribe communication with MQTT broker.

