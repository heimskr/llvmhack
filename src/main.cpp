#include <iostream>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LLVMContext.h>
#include <string>

int main() {
	llvm::DataLayout datalayout("e-m:e-i8:8:32-i16:16:32-i64:64-n32:64-S128");
	llvm::LLVMContext llvm;

	llvm::IntegerType *i24 = llvm::IntegerType::get(llvm, 24);
	llvm::IntegerType *i32 = llvm::IntegerType::get(llvm, 32);
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
}
