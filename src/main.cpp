#include <iostream>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LLVMContext.h>
#include <string>

int main() {
	llvm::DataLayout datalayout("e-m:e-i8:8:32-i16:16:32-i64:64-n32:64-S128");
	llvm::LLVMContext llvm;

	auto print_struct = [&](const char *name, llvm::StructType *s) {
		std::cerr << "\n\e[1m" << name << "\e[22m:\n\n";
		const auto *layout = datalayout.getStructLayout(s);
		std::cerr << "  hasPadding: " << std::boolalpha << layout->hasPadding() << '\n';
		std::cerr << "  getNumElements: " << s->getNumElements() << '\n';
		for (unsigned i = 0; i < s->getNumElements(); ++i) {
			std::string str;
			llvm::raw_string_ostream rso(str);
			s->elements()[i]->print(rso, true, false);

			std::cerr << "  " << i << ": \e[32m" << rso.str() << "\e[39m\n";
			std::cerr << "    offset="
			          << layout->getElementOffset(i)
			          << ", width=\n";
			// try {
				// std::cerr << s->elements()[i]->getIntegerBitWidth() << '\n';
			// } catch (...) {
				// std::cerr << "???\n";
			// }
		}
		std::cerr << "  Size: " << layout->getSizeInBytes() << "\n\n";
	};

	llvm::IntegerType *i1  = llvm::IntegerType::get(llvm, 1);
	llvm::IntegerType *i8  = llvm::IntegerType::get(llvm, 8);
	llvm::IntegerType *i16 = llvm::IntegerType::get(llvm, 16);
	llvm::IntegerType *i24 = llvm::IntegerType::get(llvm, 24);
	llvm::IntegerType *i32 = llvm::IntegerType::get(llvm, 32);
	llvm::IntegerType *i64 = llvm::IntegerType::get(llvm, 64);

	llvm::FunctionType *i32e   = llvm::FunctionType::get(i32, true);
	llvm::PointerType  *i32ep  = llvm::PointerType::get(i32e, 0);
	llvm::PointerType  *i32epp = llvm::PointerType::get(i32ep, 0);

	llvm::PointerType *i8p  = llvm::PointerType::get(i8,  0);
	llvm::PointerType *i8pp = llvm::PointerType::get(i8p, 0);

	llvm::ArrayType *i8x4 = llvm::ArrayType::get(i8, 4);
	llvm::ArrayType *i8x8 = llvm::ArrayType::get(i8, 8);

	// %"class.std::basic_ostream.base" = type { i32 (...)** }
	llvm::StructType *basic_ostream_base = llvm::StructType::get(i32epp);
	print_struct("class.std::basic_ostream.base", basic_ostream_base);

	// %"class.std::locale::facet" = type <{
	//     i32 (...)**,
	//     i32,
	//     [4 x i8]
	// }>
	llvm::StructType *locale_facet = llvm::StructType::get(llvm, {i32epp, i32, i8x4}, true);
	print_struct("class.std::locale::facet", locale_facet);
	llvm::PointerType *locale_facetp = llvm::PointerType::get(locale_facet, 0);
	llvm::PointerType *locale_facetpp = llvm::PointerType::get(locale_facetp, 0);

	// %"class.std::locale::_Impl" = type {
	//     i32,
	//     %"class.std::locale::facet"**,
	//     i64,
	//     %"class.std::locale::facet"**,
	//     i8**
	// }
	llvm::StructType *locale_impl = llvm::StructType::get(i32, locale_facetpp, i64, locale_facetpp, i8pp);
	print_struct("class.std::locale::_Impl", locale_impl);
	llvm::PointerType *locale_implp = llvm::PointerType::get(locale_impl, 0);

	// %"class.std::locale" = type { %"class.std::locale::_Impl"* }
	llvm::StructType *locale = llvm::StructType::get(locale_implp);
	print_struct("class.std::locale", locale);

	// %"class.std::basic_streambuf" = type {
	//     i32 (...)**,
	//     i8*,
	//     i8*,
	//     i8*,
	//     i8*,
	//     i8*,
	//     i8*,
	//     %"class.std::locale"
	// }
	llvm::StructType *basic_streambuf = llvm::StructType::get(i32epp, i8p, i8p, i8p, i8p, i8p, i8p, locale);
	print_struct("class.std::basic_streambuf", basic_streambuf);

	// %union.anon = type { i64, [8 x i8] }
	llvm::StructType *union_anon = llvm::StructType::get(i64, i8x8);
	print_struct("union.anon", union_anon);

	// %"struct.std::__cxx11::basic_string<char>::_Alloc_hider" = type { i8* }
	llvm::StructType *alloc_hider = llvm::StructType::get(i8p);
	print_struct("struct.std::__cxx11::basic_string<char>::_Alloc_hider", alloc_hider);

	// %"class.std::__cxx11::basic_string" = type {
	//     %"struct.std::__cxx11::basic_string<char>::_Alloc_hider",
	//     i64,
	//     %union.anon
	// }
	llvm::StructType *basic_string = llvm::StructType::get(alloc_hider, i64, union_anon);
	print_struct("class.std::__cxx11::basic_string", basic_string);

	// %"class.std::__cxx11::basic_stringbuf" = type {
	//     %"class.std::basic_streambuf",
	//     i32,
	//     %"class.std::__cxx11::basic_string"
	// }
	llvm::StructType *basic_stringbuf = llvm::StructType::get(basic_streambuf, i32, basic_string);
	print_struct("class.std::__cxx11::basic_stringbuf", basic_stringbuf);

	llvm::Type *void_type = llvm::Type::getVoidTy(llvm);
	llvm::PointerType *vp = llvm::PointerType::get(void_type, 0);
	auto *vi32vpi32 = llvm::FunctionType::get(void_type, {i32, vp, i32}, false);
	auto *vi32vpi32p = llvm::PointerType::get(vi32vpi32, 0);

	// %"struct.std::ios_base::_Callback_list" = type {
	//     %"struct.std::ios_base::_Callback_list"*,
	//     void (i32, %"class.std::ios_base"*, i32)*,
	//     i32,
	//     i32
	// }
	auto *callback_list = llvm::StructType::get(vp, vi32vpi32p, i32, i32);
	print_struct("struct.std::ios_base::_Callback_list", callback_list);
	auto *callback_listp = llvm::PointerType::get(callback_list, 0);

	// %"struct.std::ios_base::_Words" = type {
	//     i8*,
	//     i64
	// }
	auto *words = llvm::StructType::get(i8p, i64);
	print_struct("struct.std::ios_base::_Words", words);
	auto *wordsx8 = llvm::ArrayType::get(words, 8);
	auto *wordsp = llvm::PointerType::get(words, 0);

	// %"class.std::ios_base" = type {
	//     i32 (...)**,
	//     i64,
	//     i64,
	//     i32,
	//     i32,
	//     i32,
	//     %"struct.std::ios_base::_Callback_list"*,
	//     %"struct.std::ios_base::_Words",
	//     [8 x %"struct.std::ios_base::_Words"],
	//     i32,
	//     %"struct.std::ios_base::_Words"*,
	//     %"class.std::locale"
	// }
	auto *ios_base = llvm::StructType::get(i32epp, i64, i64, i32, i32, i32, callback_listp, words, wordsx8, i32, wordsp, locale);
	print_struct("class.std::ios_base", ios_base);

	// %"class.std::basic_ios" = type {
	//     %"class.std::ios_base",
	//     %"class.std::basic_ostream"*,
	//     i8,
	//     i8,
	//     %"class.std::basic_streambuf"*,
	//     %"class.std::ctype"*,
	//     %"class.std::num_put"*,
	//     %"class.std::num_get"*
	// }
	auto *basic_ios = llvm::StructType::get(ios_base, vp, i8, i8, vp, vp, vp, vp);
	print_struct("class.std::basic_ios", basic_ios);

	// %"class.std::__cxx11::basic_ostringstream" = type {
	//     %"class.std::basic_ostream.base",
	//     %"class.std::__cxx11::basic_stringbuf",
	//     %"class.std::basic_ios"
	// }
	llvm::StructType *basic_ostringstream = llvm::StructType::get(basic_ostream_base, basic_stringbuf, basic_ios);
	print_struct("class.std::__cxx11::basic_ostringstream", basic_ostringstream);

	return 0;

	/*
	llvm::StructType *s1 = llvm::StructType::get(i32, i32, i24, i24, i32, i24, i24, i32);
	const llvm::StructLayout *structlayout1 = datalayout.getStructLayout(s1);
	std::cout << "hasPadding: " << std::boolalpha << structlayout1->hasPadding() << '\n';
	std::cout << "getNumElements: " << s1->getNumElements() << '\n';
	for (unsigned i = 0; i < s1->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout1->getElementOffset(i) << ", width="
		          << s1->elements()[i]->getIntegerBitWidth() << '\n';
	}
	std::cout << "Size: " << structlayout1->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';
	llvm::IntegerType *i8 = llvm::IntegerType::get(llvm, 8);
	llvm::StructType *s2 = llvm::StructType::get(i8, i8, i24, i24, i32, i24, i24, i32);
	const llvm::StructLayout *structlayout2 = datalayout.getStructLayout(s2);
	for (unsigned i = 0; i < s2->getNumElements(); ++i) {
		std::cout << i << ": " << structlayout2->getElementOffset(i) << '\n';
	}

	std::cout << std::string(32, '=') << '\n';
	llvm::IntegerType *i1 = llvm::IntegerType::get(llvm, 1);
	llvm::IntegerType *i64 = llvm::IntegerType::get(llvm, 64);
	llvm::StructType *s3 = llvm::StructType::get(i8, i8, i24, i24, i32, i64, i24, i32, i1, i64, i1);
	const llvm::StructLayout *structlayout3 = datalayout.getStructLayout(s3);
	for (unsigned i = 0; i < s3->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout3->getElementOffset(i) << " (i"
		          << s3->elements()[i]->getIntegerBitWidth() << ")\n";
	}
	std::cout << "Size: " << structlayout3->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';
	llvm::StructType *s4 = llvm::StructType::get(i8, i8, i24, i24, i32, i32, i24, i32, i1, i32, i1);
	const llvm::StructLayout *structlayout4 = datalayout.getStructLayout(s4);
	for (unsigned i = 0; i < s4->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout4->getElementOffset(i) << " (i"
		          << s4->elements()[i]->getIntegerBitWidth() << ")\n";
	}
	std::cout << "Size: " << structlayout4->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';
	llvm::StructType *s5 = llvm::StructType::get(i1);
	const llvm::StructLayout *structlayout5 = datalayout.getStructLayout(s5);
	for (unsigned i = 0; i < s5->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout5->getElementOffset(i) << " (i"
		          << s5->elements()[i]->getIntegerBitWidth() << ")\n";
	}
	std::cout << "Size: " << structlayout5->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';
	llvm::IntegerType *i16 = llvm::IntegerType::get(llvm, 16);
	llvm::StructType *s6 = llvm::StructType::get(i32, i8, i32, i64, i16, i32);
	const llvm::StructLayout *structlayout6 = datalayout.getStructLayout(s6);
	for (unsigned i = 0; i < s6->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout6->getElementOffset(i) << " (i"
		          << s6->elements()[i]->getIntegerBitWidth() << ")\n";
	}
	std::cout << "Size: " << structlayout6->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';
	llvm::IntegerType *i17 = llvm::IntegerType::get(llvm, 17);
	llvm::ArrayType *i8x3 = llvm::ArrayType::get(i8, 3);
	llvm::ArrayType *i24x3 = llvm::ArrayType::get(i24, 3);
	llvm::ArrayType *i8x3x2 = llvm::ArrayType::get(i8x3, 2);
	llvm::StructType *s7_1 = llvm::StructType::get(i17, i8x3x2);
	llvm::PointerType *ptr_s7_1 = llvm::PointerType::get(s7_1, 0);
	llvm::PointerType *ptr_i8x3x2 = llvm::PointerType::get(i8x3x2, 0);
	std::vector<llvm::Type *> args {i8x3, s7_1};
	llvm::FunctionType *func = llvm::FunctionType::get(i8x3x2, args, false);
	llvm::StructType *s7 = llvm::StructType::get(i32, i8, i8x3, i8, i8, i8, i8, i64, i16, i32);
	const llvm::StructLayout *structlayout7 = datalayout.getStructLayout(s7);
	for (unsigned i = 0; i < s7->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout7->getElementOffset(i) << " (i"
		          << (s7->elements()[i]->isIntegerTy()? s7->elements()[i]->getIntegerBitWidth() : 0) << ")\n";
	}
	std::cout << "Size: " << structlayout7->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';

	llvm::StructType *s8 = llvm::StructType::get(i32, i8, i32, i64, i16, i32);
	const llvm::StructLayout *structlayout8 = datalayout.getStructLayout(s8);
	for (unsigned i = 0; i < s8->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout8->getElementOffset(i) << " (i"
		          << (s8->elements()[i]->isIntegerTy()? s8->elements()[i]->getIntegerBitWidth() : 0) << ")\n";
	}
	std::cout << "Size: " << structlayout8->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';
	llvm::PointerType *i8p = llvm::PointerType::get(i8, 0);
	llvm::ArrayType *i8x4 = llvm::ArrayType::get(i8, 4);
	// %"struct.std::__1::basic_string<char>::__long" = type { i64, i64, i8* }
	llvm::StructType *bslong = llvm::StructType::get(llvm, {i64, i64, i8p}, false);
	// %union.anon = type { %"struct.std::__1::basic_string<char>::__long" }
	llvm::StructType *anon = llvm::StructType::get(llvm, {bslong}, false);
	// %"struct.std::__1::basic_string<char>::__rep" = type { %union.anon }
	llvm::StructType *bsrep = llvm::StructType::get(llvm, {anon}, false);
	// %"struct.std::__1::__compressed_pair_elem" = type { %"struct.std::__1::basic_string<char>::__rep" }
	llvm::StructType *cpe = llvm::StructType::get(llvm, {bsrep}, false);
	// %"class.std::__1::__compressed_pair" = type { %"struct.std::__1::__compressed_pair_elem" }
	llvm::StructType *compressed_pair = llvm::StructType::get(llvm, {cpe}, false);
	// %"class.std::__1::basic_string" = type { %"class.std::__1::__compressed_pair" }
	llvm::StructType *basic_string = llvm::StructType::get(llvm, {compressed_pair}, false);
	// %"struct.std::__1::pair" = type <{ %"class.std::__1::basic_string", i32, [4 x i8] }>
	llvm::StructType *s9 = llvm::StructType::get(llvm, {basic_string, i32, i8x4}, true);
	
	const llvm::StructLayout *structlayout9 = datalayout.getStructLayout(s9);
	for (unsigned i = 0; i < s9->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout9->getElementOffset(i);
		if (s9->elements()[i]->isIntegerTy())
			std::cout << " (i" << s9->elements()[i]->getIntegerBitWidth() << ')';
		std::cout << '\n';
	}
	std::cout << "Size (9): " << structlayout9->getSizeInBytes() << '\n';

	std::cout << std::string(32, '=') << '\n';
	llvm::ArrayType *i8x8 = llvm::ArrayType::get(i8, 8);
	llvm::ArrayType *i8x32 = llvm::ArrayType::get(i8, 32);
	// %"struct.std::__1::aligned_storage<24, 16>::type"
	llvm::StructType *aligned24_16 = llvm::StructType::get(llvm, {i8x32}, false);
	// %struct.StorageDevice
	llvm::StructType *device = llvm::StructType::get(llvm, {i8p}, false); // Actually {i32 (...)**}
	llvm::PointerType *device_ptr = llvm::PointerType::get(device, 0);
	// %"class.std::__1::__function::__value_func"
	llvm::StructType *s10 = llvm::StructType::get(llvm, {aligned24_16, device_ptr, i8x8}, false);

	const llvm::StructLayout *structlayout10 = datalayout.getStructLayout(s10);
	for (unsigned i = 0; i < s10->getNumElements(); ++i) {
		std::cout << i << ": offset=" << structlayout10->getElementOffset(i);
		if (s10->elements()[i]->isIntegerTy())
			std::cout << " (i" << s10->elements()[i]->getIntegerBitWidth() << ')';
		std::cout << '\n';
	}
	std::cout << "Size (10): " << structlayout10->getSizeInBytes() << '\n';
	//*/
}
