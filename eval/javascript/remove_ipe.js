importPackage(Packages.com.ti.debug.engine.scripting);
importPackage(Packages.com.ti.ccstudio.scripting.environment);
importPackage(Packages.java.lang);

var script = ScriptingEnvironment.instance();
var ds = script.getServer( "DebugServer.1" );
ds.setConfig("../javascript/MSP430FR5994.ccxml");
debugSession = ds.openSession( ".*" );

debugSession.options.setBoolean("IPEraseOnConnect",true);
debugSession.options.setBoolean("MSP430LPMx5Mode",false);
//debugSession.options.printOptions(".*");

debugSession.target.connect();
debugSession.target.disconnect();
