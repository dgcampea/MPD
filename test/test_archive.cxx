#include "config.h"
#include "ArchiveLookup.hxx"
#include "Compiler.h"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <glib.h>

#include <string.h>

class ArchiveLookupTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ArchiveLookupTest);
	CPPUNIT_TEST(TestArchiveLookup);
	CPPUNIT_TEST_SUITE_END();

public:
	void TestArchiveLookup();
};

void
ArchiveLookupTest::TestArchiveLookup()
{
	char *archive, *inpath, *suffix;

	char *path = strdup("");
	CPPUNIT_ASSERT_EQUAL(false,
			     archive_lookup(path, &archive, &inpath, &suffix));
	g_free(path);

	path = strdup(".");
	CPPUNIT_ASSERT_EQUAL(false,
			     archive_lookup(path, &archive, &inpath, &suffix));
	g_free(path);

	path = strdup("config.h");
	CPPUNIT_ASSERT_EQUAL(false,
			     archive_lookup(path, &archive, &inpath, &suffix));
	g_free(path);

	path = strdup("src/foo/bar");
	CPPUNIT_ASSERT_EQUAL(false,
			     archive_lookup(path, &archive, &inpath, &suffix));
	g_free(path);

	path = strdup("Makefile/foo/bar");
	CPPUNIT_ASSERT_EQUAL(true,
			     archive_lookup(path, &archive, &inpath, &suffix));
	CPPUNIT_ASSERT_EQUAL(path, archive);
	CPPUNIT_ASSERT_EQUAL(0, strcmp(archive, "Makefile"));
	CPPUNIT_ASSERT_EQUAL(0, strcmp(inpath, "foo/bar"));
	CPPUNIT_ASSERT_EQUAL((char *)nullptr, suffix);
	g_free(path);

	path = strdup("config.h/foo/bar");
	CPPUNIT_ASSERT_EQUAL(true,
			     archive_lookup(path, &archive, &inpath, &suffix));
	CPPUNIT_ASSERT_EQUAL(path, archive);
	CPPUNIT_ASSERT_EQUAL(0, strcmp(archive, "config.h"));
	CPPUNIT_ASSERT_EQUAL(0, strcmp(inpath, "foo/bar"));
	CPPUNIT_ASSERT_EQUAL(0, strcmp(suffix, "h"));
	g_free(path);
}

CPPUNIT_TEST_SUITE_REGISTRATION(ArchiveLookupTest);

int
main(gcc_unused int argc, gcc_unused char **argv)
{
	CppUnit::TextUi::TestRunner runner;
	auto &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	return runner.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
