This project is a compiler for FJ+-

To compile run
  make

Then you are ready to run you FJ+- code, like
```java
class Int extends Object{
    Int(){super();}

    Int add(Int rhs){
        return rhs.add(this);
    }
}

class O extends Int{
    O(){super();}

    Int add(Int rhs){
        return rhs;
    }
}

class S extends Int{
    Int num;

    S(Int num){
        super();
        this.num=num;
    }

    Int add(Int rhs){
        return this.num.add(new S(rhs));
    }

}
new S(new S(new O())).add(new S(new O()))
```
