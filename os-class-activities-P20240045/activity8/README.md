# Class Activity 8 - Memory Management & Virtual Memory

* **Student Name:** PI Sereyvathanak
* **Student ID:** P20240045
* **Personalization:** a = 5, b = 4 → N = (10a+b) mod 128 = 54
* **Programming Language Used:** Java

---

## Part A1 — Address Translation (by Hand)

### Translation Table

| Logical Address (LA) | Page = LA/16 | Offset = LA%16 | Valid? | Frame | Physical Address |
| -------------------- | ------------ | -------------- | ------ | ----- | ---------------- |
| 20                   | 1            | 4              | Yes    | 2     | 2×16+4 = 36      |
| 100                  | 6            | 4              | Yes    | 0     | 0×16+4 = 4       |
| 48                   | 3            | 0              | No     | -     | Page Fault       |
| 16                   | 1            | 0              | Yes    | 2     | 2×16+0 = 32      |
| 127                  | 7            | 15             | Yes    | 4     | 4×16+15 = 79     |
| 54                   | 3            | 6              | No     | -     | Page Fault       |

### Questions

#### 1. Offset unchanged because:

The offset identifies the exact byte location inside a page. During address translation, only the page number is mapped to a frame number. Since pages and frames have the same size, the offset remains unchanged.

#### 2. Largest offset and bits required

Page size = 16 bytes

Largest offset:

16 − 1 = 15

Bits required:

log₂(16) = 4 bits

**Answer:** Largest offset = **15**, Offset bits = **4 bits**

#### 3. Internal Fragmentation

Process size:

60 + a = 60 + 5 = 65 bytes

Pages required:

⌈65 / 16⌉ = 5 pages

Allocated memory:

5 × 16 = 80 bytes

Internal fragmentation:

80 − 65 = 15 bytes

**Answer:** 5 pages allocated, 15 bytes internal fragmentation.

---

## Part A2 — Page Replacement (by Hand)

### My Reference String

5 0 1 2 0 3 0 4 2 3 0 3

### Prediction

I predict that **LRU** will have fewer page faults than **FIFO** because LRU keeps recently used pages in memory, while FIFO only considers arrival order.

---

### FIFO Trace

| Ref | H/F | F1 | F2 | F3 | Evicted |
| --- | --- | -- | -- | -- | ------- |
| 5   | F   | 5  | -  | -  | -       |
| 0   | F   | 5  | 0  | -  | -       |
| 1   | F   | 5  | 0  | 1  | -       |
| 2   | F   | 2  | 0  | 1  | 5       |
| 0   | H   | 2  | 0  | 1  | -       |
| 3   | F   | 2  | 3  | 1  | 0       |
| 0   | F   | 2  | 3  | 0  | 1       |
| 4   | F   | 4  | 3  | 0  | 2       |
| 2   | F   | 4  | 2  | 0  | 3       |
| 3   | F   | 4  | 2  | 3  | 0       |
| 0   | F   | 0  | 2  | 3  | 4       |
| 3   | H   | 0  | 2  | 3  | -       |

**Total FIFO Faults = 10**

---

### LRU Trace

| Ref | H/F | F1 | F2 | F3 | Evicted |
| --- | --- | -- | -- | -- | ------- |
| 5   | F   | 5  | -  | -  | -       |
| 0   | F   | 5  | 0  | -  | -       |
| 1   | F   | 5  | 0  | 1  | -       |
| 2   | F   | 2  | 0  | 1  | 5       |
| 0   | H   | 2  | 0  | 1  | -       |
| 3   | F   | 2  | 0  | 3  | 1       |
| 0   | H   | 2  | 0  | 3  | -       |
| 4   | F   | 4  | 0  | 3  | 2       |
| 2   | F   | 4  | 0  | 2  | 3       |
| 3   | F   | 4  | 3  | 2  | 0       |
| 0   | F   | 0  | 3  | 2  | 4       |
| 3   | H   | 0  | 3  | 2  | -       |

**Total LRU Faults = 9**

### Comparison

FIFO produced 10 page faults while LRU produced 9 page faults. LRU performed better than FIFO and matched my prediction.

---

## Part B — Simulator Verification

![Translation](screenshots/task1_translation.png)

![FIFO](screenshots/task2_fifo.png)

![LRU](screenshots/task2_lru.png)

### Verification Results

* The simulator matched all entries in my A1 translation table.
* The simulator produced the same FIFO and LRU fault counts as my hand traces.
* FIFO faults = 10
* LRU faults = 9

No discrepancies were found between the simulator and the manual calculations.

---

## Part C — Applied Reasoning

### 1. Why is paging free of external fragmentation while contiguous allocation is not?

Paging divides memory into fixed-size pages and frames. Any free frame can hold any page, so memory does not become fragmented into unusable gaps. In contiguous allocation, a process needs one continuous block of memory, which can leave many small free spaces that cannot be used effectively.

### 2. Why does loading a page into an empty frame still count as a page fault?

A page fault occurs whenever the requested page is not currently in memory. Even if an empty frame exists, the operating system must still fetch the page from disk into memory before execution can continue.

### 3. Why did LRU and FIFO differ on my reference string?

They differed because LRU considers recent usage while FIFO only considers arrival order. For example, page 0 was referenced multiple times. LRU kept recently used pages in memory, while FIFO sometimes removed pages simply because they had been loaded earlier.

### 4. What is thrashing, and what would happen with only one frame?

Thrashing occurs when the operating system spends most of its time swapping pages in and out of memory instead of executing processes. With only one frame, almost every page reference would cause a page fault because the working set requires multiple pages. System performance would become very poor.

### 5. Give one benefit and one risk of demand paging.

**Benefit:** Memory is used more efficiently because only needed pages are loaded.

**Risk:** The first access to a page causes a page fault, which introduces delay while the page is loaded from secondary storage.

---

## Conclusion

The activity demonstrated how logical addresses are translated into physical addresses using paging and how FIFO and LRU page replacement algorithms manage memory. The simulator results matched the hand calculations, confirming the correctness of the manual traces and reasoning.
