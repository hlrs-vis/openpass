\page execution Execution

\tableofcontents


\subsection execution_openpassmaster OpenPassMaster

\subsubsection execution_openpassmaster_cla Command Line Arguments

The master can be configured by applying the following command line argument.  
If it is unspecified it will be defaulted (*default values in []*).

The master supports the following argument:

* `-c / --config [masterConfig.xml]`  
  The configuration for calling the slaves.

Example:  
```bash
OpenPassMaster -c/--config [masterConfig.xml]
```

\subsubsection execution_openpassmaster_masterconfiguration Master Configuration

The following snippet shows the `masterConfig.xml` with the default values for the optional tags.

```xml
<masterConfig>
  <!-- Value between 0 (no logging) and 5 (maximum = debug core) [see also note 1]-->
  <logLevel>0</logLevel>
  <!-- Filename of the masters logfile -->
  <logFileMaster>OpenPassMaster.log</logFileMaster>
  <!-- Name of the executable -->
  <slave>OpenPassSlave</slave>
  <!-- Path of the libraries used by the slave [see also note 2] -->
  <libraries>lib</libraries>
  <slaveConfigs>
    <!-- first slave instance -->
    <slaveConfig>
      <!-- Filename of the slaves logfile --> 
      <logFileSlave>OpenPassSlave.log</logFileSlave>
      <!-- Where to find experiment configuration -->
      <configurations>configs</configurations>
      <!-- Where to output results -->
      <results>results</results>
    </slaveConfig>
    
    <!-- second slave configuration -->
    <slaveConfig>
      ... 
    <slaveConfig>      
  </slaveConfigs>
</masterConfig>
```
__Comments:__  
1. Note that the level is set for the master __as well__ as the the slaves it calls.  
2. Note that the path to the libraries is relative to the path of the executable unless specified by an absolute path.

The given configuration will lead to the following equivalent output:
```bash
# first slave instance
OpenPassSlave --logLevel 0 --logFile OpenPassSlave.log --lib lib --results results --configs configs

# second slave instance
OpenPassSlave ...
```

__Minimum Configuration:__  
All values can be defaulted, so the minimum configuration would simply contain:
```xml
<masterConfig>
  <slaveConfigs>
    <slaveConfig/>
  <slaveConfigs>
<masterConfig>
```

__Equivalent Output:__
```bash
OpenPassSlave --logLevel 0 --logFile OpenPassSlave.log --lib lib --configs configs --results results
```

_Note that this is identical to a call to `OpenPassSlave` without arguments, so calling the master without any configuration is not allowed._

\subsection execution_openpassslave OpenPassSlave

\subsubsection execution_openpassslave_cla Command Line Arguments

The slave can be configured by applying the following command line arguments.  
Unspecified arguments will be defaulted (*default values in []*).

The slave supports the following arguments:

* `--logLevel [0]`  
  Logging level between 0 (minimum) and 5 (maximum - debug core)
* `--logFile [OpenPassSlave.log]`  
  Name of the log file
* `--lib [lib]`  
  Path of the libraries (relative or absolute)
* `--configs [configs]`  
  Path for writing outputs (relative or absolute)
* `--results [results]`  
  Path for writing outputs (relative or absolute)

\subsubsection execution_openpassslave_libs Library Selection

The slave uses the following core libraries:

* EventDetector
* Manipulator
* SpawnPoint
* Stochastics
* World
* Observation

The core retrieves this information from the `<Libraries>` tag in the `slaveConfig.xml` located in the specified configuration path:

```xml
<slaveConfig>
  ...
  <ExperimentConfig>
    <Libraries>
      <EventDetectorLibrary>EventDetector</EventDetectorLibrary>
      <ManipulatorLibrary>Manipulator</ManipulatorLibrary>
      <SpawnPointLibrary>SpawnPoint</SpawnPointLibrary>
      <StochasticsLibrary>Stochastics</StochasticsLibrary>
      <WorldLibrary>World</WorldLibrary>
      <ObservationLibrary>Observation</ObservationLibrary>
    </Libraries>
  </ExperimentConfig>
  ...
</slaveConfig>
```

If a mandatory library is missing, the slave tries to load a defaulted one, which is for e.g. `<SpawnPointLibrary>` `SpawnPoint.dll` under Windows or `libSpawnPoint.so` under Linux, respectively.
In such cases, a warning will be pushed to the log each.
