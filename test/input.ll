; ModuleID = 'test'
source_filename = "test"

define i32 @test(i32 %x) {
entry:
  %mul = mul i32 %x, 2
  ret i32 %mul
}
