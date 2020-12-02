import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.lang.instrument.ClassDefinition;
import java.lang.instrument.Instrumentation;
import java.lang.instrument.UnmodifiableClassException;
import java.util.ArrayList;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

/**
 * Java hotagent, compiled with Java8, build to jar file, eg:./lib/hotagent.jar,
 * add jvm arguments: -javaagent:./lib/hotagent.jar
 */
public class ClassReloader {

	private static Instrumentation inst = null;

	public ClassReloader() {

	}

	public static void premain(String args, Instrumentation inst) {
		ClassReloader.inst = inst;
	}

	public static void reload(Class<?> cls, File file)
			throws ClassNotFoundException, IOException, UnmodifiableClassException {
		byte[] code = loadBytes(cls, file);
		if (code == null) {
			throw new IOException("unknown file");
		} else {
			ClassDefinition def = new ClassDefinition(cls, code);
			inst.redefineClasses(new ClassDefinition[] { def });
			System.out.println(cls.getName() + " reloaded");
		}
	}

	public static void reloadClasses(List<Class<?>> classes, List<File> files)
			throws ClassNotFoundException, IOException, UnmodifiableClassException {
		List<ClassDefinition> defList = new ArrayList<>();
		for (int i = 0; i < classes.size() && i < files.size(); i++) {
			byte[] code = loadBytes(classes.get(i), files.get(i));
			if (code == null) {
				throw new IOException("unknown file");
			}
			ClassDefinition def = new ClassDefinition(classes.get(i), code);
			defList.add(def);
		}
		if (defList.isEmpty()) {
			return;
		}
		ClassDefinition[] defArr = new ClassDefinition[defList.size()];
		for (int i = 0; i < defList.size(); i++) {
			defArr[i] = defList.get(i);
		}
		inst.redefineClasses(defArr);
	}

	private static byte[] loadBytes(Class<?> cls, File file) throws ClassNotFoundException, IOException {
		String name = file.getName();
		if (name.endsWith(".jar")) {
			return loadBytesFromJarFile(cls, file);
		} else {
			return name.endsWith(".class") ? loadBytesFromClassFile(file) : null;
		}
	}

	private static byte[] loadBytesFromJarFile(Class<?> cls, File file) throws IOException, ClassNotFoundException {
		JarFile jarFile = new JarFile(file);
		String name = cls.getName();
		name = name.replaceAll("\\.", "/") + ".class";
		JarEntry entry = jarFile.getJarEntry(name);
		if (entry == null) {
			throw new ClassNotFoundException(name);
		} else {
			byte[] buffer = new byte[(int) entry.getSize()];
			try (BufferedInputStream bis = new BufferedInputStream(jarFile.getInputStream(entry));) {
				bis.read(buffer);
			} catch (IOException e) {
				throw e;
			}
			return buffer;
		}
	}

	private static byte[] loadBytesFromClassFile(File file) throws IOException {
		byte[] buffer = new byte[(int) file.length()];
		FileInputStream fis = new FileInputStream(file);
		try (BufferedInputStream bis = new BufferedInputStream(fis);) {
			bis.read(buffer);
		} catch (IOException e) {
			throw e;
		}
		return buffer;
	}

}
